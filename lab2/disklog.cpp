#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include "const.hpp"
#include "disklog.hpp"

using namespace std;

Disklog::Disklog(){
	fd = 0;
	mem = 0;
	sb = 0;
	gen = 0;
	tail = 0;
}

Disklog::~Disklog(){
	munmap(mem, BLK_SIZE * N_LOG_BLK);
	if (fd != 0){
		close(fd);
	}
}

int Disklog::open(string &devfile){
	// open device
	fd = ::open(devfile.c_str(), O_RDWR);
	if (fd == -1){
		perror("open() in Disklog::open()"); 
		return 0;
	}

	// mmap the device
	mem = (uint8_t*)mmap(0, BLK_SIZE * N_LOG_BLK, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (mem == MAP_FAILED){
		perror("mmap() in Disklog::open()");
		return 0;
	}

	// assign the superblock
	sb = (Superblock*) mem;

	// assign the logblock
	lb = (Logblock*) (mem + BLK_SIZE);

	return 1;
}

// add an entry, return 1 on success, 0 if no space
int Disklog::add_entry(uint32_t opcode, uint64_t a, uint64_t b){
	int ret;
	// if the current logblock has space, write to it
	if (lb[tail].enough_space()){
		lb[tail].add_entry(opcode, a, b);
		ret = msync(lb + tail, BLK_SIZE, MS_SYNC);
		if (ret == -1){
			perror("msync() in Disklog::add_entry()");
		}
		return 1;
	}

	// if still have free logblock, write to it
	if (tail + 1 < log_size){
		tail ++;
		lb[tail].reset();
		lb[tail].gen = gen;
		lb[tail].update_chksum();
		lb[tail].add_entry(opcode, a, b);
		ret = msync(lb + tail, BLK_SIZE, MS_SYNC);
		if (ret == -1){
			perror("msync() in Disklog::add_entry()");
		}
		return 1;
	}

	// no space, return 0
	return 0;
}

// test if there is enough space for a new entry
int Disklog::enough_space(){
	return lb[tail].enough_space() || tail + 1 < log_size;
}

// return 1 if valid, 0 otherwise
int Disklog::valid(){
	// test if the superblock is valid or not
	int ret = sb->is_valid();
	return ret;
}

// increment the generation number
void Disklog::inc_gen(int clear_checkpoint){
	int ret;
	//increment the generation number in the superblock, and update checksum
	sb->gen++;
	if (clear_checkpoint)
		sb->has_checkpoint = 0;
	sb->update_chksum();
	// flush to disk
	ret = msync(sb, BLK_SIZE, MS_SYNC);
	if (ret == -1){
		perror("msync() in Disklog::inc_gen()");
	}

	// read in-memory variables from disk
	keep_variable_in_memory();

	// reset the first logblock
	lb[0].reset();
	lb[0].gen = gen;
	lb[0].update_chksum();
}

// reset the log
void Disklog::reset(){
	int ret;
	// reset the superblock
	sb->gen = 0;
	sb->log_start = 1;
	sb->log_size = N_LOG_BLK - 1;
	sb->has_checkpoint = 0;
	sb->update_chksum();
	// flush to disk
	ret = msync(sb, BLK_SIZE, MS_SYNC);
	if (ret == -1){
		perror("msync() in Disklog::reset()");
	}

	// read in-memory variables from disk
	keep_variable_in_memory();

	// reset the first logblock
	lb[0].reset();
	lb[0].gen = gen;
	lb[0].update_chksum();
}

// set if having checkpoint
void Disklog::set_checkpoint(int has_checkpoint){
	sb->has_checkpoint = has_checkpoint;
	sb->update_chksum();
	// flush to disk
	int ret = msync(sb, BLK_SIZE, MS_SYNC);
	if (ret == -1){
		perror("msync() in Disklog::set_checkpoint()");
	}
}

// get if having checkpoint
int Disklog::get_checkpoint(){
	return sb->has_checkpoint;
}

// read in-memory variables from disk
void Disklog::keep_variable_in_memory(){
	// update in memory variables
	gen = sb->gen;
	tail = 0;
	log_size = N_LOG_BLK - 1;
}

uint64_t Disklog::get_gen(){
	return gen;
}

// ===== below are Disklog::iterator functions =====
// constructor
Disklog::iterator::iterator(){
	i = 0;
	disklog = 0;
	valid = false;
}
Disklog::iterator::iterator(int _i, Disklog* _disklog){
	i = _i;
	disklog = _disklog;
	if (i < disklog->log_size && disklog->lb[i].is_valid() && disklog->lb[i].gen == disklog->gen){
		it = disklog->lb[i].begin();
		valid = true;
	} else {
		//printf("i = %d, disklog->log_size =%d\n", i, disklog->log_size);
		//printf("disklog->lb[i].is_valid() = %d\n",disklog->lb[i].is_valid());
		valid = false;
	}
}

// test if the iterator ended
bool Disklog::iterator::end(){
	return !valid || it.end();
}

// move to the next entry
void Disklog::iterator::next(){
	it.next();
	if (it.end()){
		i++;
		if (i < disklog->log_size && disklog->lb[i].is_valid() && disklog->lb[i].gen == disklog->gen)
			it = disklog->lb[i].begin();
		else {
			//printf("i = %d, disklog->log_size =%d\n", i, disklog->log_size);
			//printf("disklog->lb[i].is_valid() = %d\n",disklog->lb[i].is_valid());
			valid = false;
		}
	}
}

// the entry pointed by the iterator
Logentry* Disklog::iterator::entry(){
	return it.entry();
}

// get an iterator
Disklog::iterator Disklog::begin(){
	return Disklog::iterator(0, this);
}
