#ifndef CHECKPOINT_HPP
#define CHECKPOINT_HPP

#include <sys/types.h>

class CheckPoint{
	private:
		int access_ckpoint(uint64_t cache[], int cache_length, int offset, int opt);
		int restore();
	public:
		int checkpoint();
};
#endif /* CHECKPOINT_HPP */
