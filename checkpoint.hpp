#ifndef CHECKPOINT_HPP
#define CHECKPOINT_HPP

#include <sys/types.h>

class CheckPoint{
	private:
		//int access_ckpoint(uint64_t cache[], int cache_length, int offset, int opt);
		//int restore();
		char* devfile; // block device path, e.g. "/dev/sdb/"
		int fd; // file descriptor
		char buf[BLK_SIZE]; // the buffer to hold write; write to disk every BLK_SIZE
		int buf_tail; // point to the tail of the buffer in form of BYTES
	public:
		//int checkpoint();
		int init(char* devfile, bool read_mode=False); // init the fd, buf, buf_tail, devfile
		int write(uint64_t val); // called by other objects (graph), to perform a buffered one uint64_t write to disk
		int read(uint64_t *ret, int len); // len is the number of uint64_t-s, ret is the return buffer
		int checkin(); // let Graph call this at last to dump the buffer leftovers onto disk
		~CheckPoint() { close(fd); };
};
#endif /* CHECKPOINT_HPP */
