#ifndef CHECKPOINT_HPP
#define CHECKPOINT_HPP

#include <sys/types.h>

class CheckPoint{
	private:
		//int access_ckpoint(uint64_t cache[], int cache_length, int offset, int opt);
		//int restore();
		int fd; // file descriptor
		char buf[BLK_SIZE]; // the buffer to hold write; write to disk every BLK_SIZE
		int buf_tail; // point to the tail of the buffer
	public:
		//int checkpoint();
		int init(); // init the fd, buf, buf_tail
		int write(string &s); // called by other objects (graph), to write to disk
		int read(char* ret, int len); // len is the number of bytes, ret is the return buffer
};
#endif /* CHECKPOINT_HPP */
