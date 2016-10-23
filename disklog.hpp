#ifndef DISKLOG_HPP
#define DISKLOG_HPP

#include <stdint.h>
#include <string>

#include "superblock.hpp"
#include "logblock.hpp"

class Disklog{
	private:
		int fd;
		uint8_t *mem;
		Superblock *sb;
		Logblock *lb;
		uint32_t gen;
		uint64_t tail;
		uint64_t log_size;
	public:
		Disklog();
		~Disklog();

		// return 0 if fail, 1 on success
		int open(std::string &_devfile);

		// add an entry, return 1 on success, 0 if no space
		int add_entry(uint32_t opcode, uint64_t a, uint64_t b);

		// test if there is enough space for a new entry
		int enough_space();

		// return 1 if valid, 0 otherwise
		int valid();

		// increment the generation number
		void inc_gen();

		// reset the log
		void reset();

		// read in-memory variables from disk
		void keep_variable_in_memory();

		class iterator{
			private:
				int i;
				Logblock::iterator it;
				bool valid;
				Disklog* disklog;
			public:
				// constructor
				iterator();
				iterator(int _i, Disklog* _disklog);
				bool end();
				void next();
				Logentry* entry();
		};

		// get an iterator
		iterator begin();
};

#endif /* DISKLOG_HPP */
