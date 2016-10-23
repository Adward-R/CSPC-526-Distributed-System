#include "logblock.hpp"
#include "const.hpp"

// ===== below are Logblock::iterator functions =====
// constructor
Logblock::iterator::iterator(){
	i = 0;
	lb = 0;
}
Logblock::iterator::iterator(int _i, Logblock *_lb){
	i = _i;
	lb = _lb;
}

// test if exceeds the last entry
bool Logblock::iterator::end(){
	return i + LOG_ENTRY_SIZE > lb->n * LOG_ENTRY_SIZE;
}

// move to the next entry
void Logblock::iterator::next(){
	i += LOG_ENTRY_SIZE;
}

// the entry pointed by the iterator
Logentry* Logblock::iterator::entry(){
	return (Logentry*)(lb->entry_area + i);
}

// ===== below are Logblock functions =====
// get an iterator
Logblock::iterator Logblock::begin(){
	return Logblock::iterator(0, this);
}

// reset the logblock
void Logblock::reset(){
	gen = 0;
	n = 0;
}

// test if there is enough space for a new entry
int Logblock::enough_space(){
	return n < N_ENTRY_PER_LOGBLOCK;
}

// add an entry, return 0 on fail (e.g. full), 1 on success
int Logblock::add_entry(uint32_t opcode, uint64_t a, uint64_t b){
	// if no space, return 0
	if (!enough_space())
		return 0;

	// get the corresponding entry
	Logentry *l = (Logentry*)(entry_area + n * LOG_ENTRY_SIZE);

	// get a pointer t aligned to 64 bits, and a pointer to the 64bits of field n
	uint64_t *t = (uint64_t*)(((uint64_t)l) / sizeof(uint64_t) * sizeof(uint64_t)), *first = (uint64_t*)this;
	// add the entry and update chksum
	chksum ^= *first ^ t[0] ^ t[1] ^ t[2];
	l->a = a;
	l->b = b;
	l->opcode = opcode;
	n++;
	chksum ^= *first ^ t[0] ^ t[1] ^ t[2];

#if 0
	// if full, update the checksum
	if (n >= N_ENTRY_PER_LOGBLOCK){
		update_chksum();
	}
#endif
	return 1;
}

// calculate the checksum, not including the chksum field
uint64_t Logblock::calc_chksum(){
	uint64_t *buf = (uint64_t*)this;
	uint64_t xorsum = chksum; // to cancel out the chksum field
	for (int i = 0; i < BLK_SIZE/sizeof(uint64_t); i++){
		xorsum ^= buf[i];
	}
	return xorsum;
}

// update the checksum
void Logblock::update_chksum(){
	chksum = calc_chksum();
}

// check if the logblock is valid (1) or not (0)
int Logblock::is_valid(){
	return calc_chksum() == chksum;
}
