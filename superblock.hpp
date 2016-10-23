#ifndef SUPERBLOCK_HPP
#define SUPERBLOCK_HPP
#include <stdint.h>
struct Superblock{
	uint64_t chksum; // checksum
	uint32_t gen; // generation number
	uint32_t log_start; // the first block of the log, not including the super block
	uint32_t log_size; // number of blocks in the log, including the superblock
	uint8_t empty[4076]; // padding

	// check if the superblock is valid or not
	bool is_valid();

	// calculate the checksum, not including the bytes of checksum field
	uint64_t calc_chksum();
};
#endif /* SUPERBLOCK_HPP */
