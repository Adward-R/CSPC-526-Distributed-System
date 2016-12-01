#include "superblock.hpp"
#include "const.hpp"
#include <cstdio>

// check if the superblock is valid or not
int Superblock::is_valid(){
	return calc_chksum() == chksum;
}

// calculate the checksum, not including the bytes of checksum field
uint64_t Superblock::calc_chksum(){
	uint64_t *buf = (uint64_t*)this;
	uint64_t xorsum = chksum; // to cancel out the chksum field
	for (int i = 0; i < BLK_SIZE/sizeof(uint64_t); i++){
		xorsum ^= buf[i];
	}
	return xorsum;
}

// update the checksum of the superblock
void Superblock::update_chksum(){
	chksum = calc_chksum();
}
