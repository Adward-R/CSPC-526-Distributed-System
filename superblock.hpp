#ifndef SUPERBLOCK_HPP
#define SUPERBLOCK_HPP
#include <stdint.h>
struct Superblock{
	uint64_t chksum; // checksum
	uint32_t gen; // generation number
	uint32_t log_start; // the first block of the log, not including the super block
	uint32_t log_size; // number of blocks in the log, not including the superblock
	uint32_t has_checkpoint; // if there is a checkpoint of not
	uint8_t empty[4072]; // padding

	// check if the superblock is valid (return 1) or not (0)
	int is_valid();

	// calculate the checksum, not including the bytes of checksum field
	uint64_t calc_chksum();

	// update the checksum of the superblock
	void update_chksum();
};
#endif /* SUPERBLOCK_HPP */
