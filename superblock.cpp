#include "superblock.hpp"
#include "const.hpp"
#include <cstdio>

// check if the superblock is valid or not
bool Superblock::is_valid(){
	uint64_t *buf = (uint64_t*)this;
	uint64_t xorsum = 0;
	for (int i = 0; i < BLK_SIZE/sizeof(uint64_t); i++){
		xorsum ^= buf[i];
	}
	return xorsum == 0;
}

