#ifndef Logblock_HPP
#define Logblock_HPP

#include <stdint.h>

#define LOG_ENTRY_SIZE 20 // must use this, not sizeof(Logentry), because the sizeof returns a 8-byte aligned value, which wastes the 4 bytes at the end of each entry

struct Logentry{
	uint64_t a, b;
	uint32_t opcode;
};

#define N_BYTE_FOR_LOG_ENTRY 4080
#define N_ENTRY_PER_LOGBLOCK (N_BYTE_FOR_LOG_ENTRY / LOG_ENTRY_SIZE)

struct Logblock{
	uint32_t gen; // generation number
	uint32_t n; // number of entries
	uint64_t chksum;
	uint8_t entry_area[N_BYTE_FOR_LOG_ENTRY];

	class iterator{
		private:
			int i;
			Logblock *lb;
		public:
			// constructor
			iterator();
			iterator(int _i, Logblock *_lb);

			// test if exceeds the last entry
			bool end();

			// move to the next entry
			void next();

			// the entry pointed by the iterator
			Logentry* entry();
	};

	// get an iterator
	iterator begin();

	// reset the logblock
	void reset();

	// test if there is enough space for a new entry
	int enough_space();

	// add an entry, return 0 on fail (e.g. full), 1 on success
	int add_entry(uint32_t opcode, uint64_t a, uint64_t b);

	// calculate the checksum, not including the chksum field
	uint64_t calc_chksum();

	// update the checksum
	void update_chksum();

	// check if the logblock is valid (1) or not (0)
	int is_valid();
};

#endif /* Logblock_HPP */
