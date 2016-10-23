#ifndef DISKLOG_HPP
#define DISKLOG_HPP

#include <stdint.h>
#include <string>

#include "superblock.hpp"

class Disklog{
	private:
		int fd;
		Superblock *sb;
	public:
		Disklog();
		~Disklog();
		int open(std::string &_devfile);
		int add_entry(uint32_t opcode, uint64_t a, uint64_t b);
		int enough_space();

		// return 1 if valid, 0 otherwise
		int valid();

		// increment the generation number
		void inc_gen();

		// reset the superblock
		void reset();

};

#endif /* DISKLOG_HPP */
