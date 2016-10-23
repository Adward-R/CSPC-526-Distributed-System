#ifndef Logblock_HPP
#define Logblock_HPP

#include <stdint.h>

#define LOG_ENTRY_SIZE 20

struct Logentry{
	uint64_t a, b;
	uint32_t opcode;
};

#define N_BYTE_FOR_LOG_ENTRY 4080

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
			iterator(int _i, Logblock *_lb);
			bool end();
			void next();
			Logentry* entry();
	};

	iterator begin();
};

#endif /* Logblock_HPP */
