#ifndef SUPERBLOCK_HPP
#define SUPERBLOCK_HPP
#include <stdint.h>
struct Superblock{
	uint64_t chksum;
	uint32_t gen;
	uint32_t log_start;
	uint32_t log_size;
	uint8_t empty[4076];

	// check if the superblock is valid or not
	bool is_valid();
};
#endif /* SUPERBLOCK_HPP */
