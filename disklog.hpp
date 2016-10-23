#ifndef DISKLOG_HPP
#define DISKLOG_HPP

#include <stdint.h>

class Disklog{
	public:
		int add_entry(uint32_t opcode, uint64_t a, uint64_t b);
};

#endif /* DISKLOG_HPP */
