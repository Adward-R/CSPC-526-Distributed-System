#include "const.hpp"
#include "disklog.hpp"

Disklog::Disklog(){
	fd = 0;
	sb = 0;
}

Disklog::~Disklog(){
	if (fd != 0){
		close(fd);
	}
}

int Disklog::open(std::string &_devfile){
	//TODO, open device
	//TODO, mmap the device
}

int Disklog::add_entry(uint32_t opcode, uint64_t a, uint64_t b){
	//TODO, write the entry to the log
}

int Disklog::enough_space(){
	//TODO, test if the log has enough space
}

// return 1 if valid, 0 otherwise
int Disklog::valid(){
	//TODO, test if the superblock is valid or not
}

// increment the generation number
void inc_gen(){
	//TODO, increment the generation number in the superblock
}

void Disklog::reset(){
	sb->gen = 0;
	sb->log_start = 1;
	sb->log_size = N_LOG_BLK;
}
