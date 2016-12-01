#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>
#include <cstdio>

#include "checkpoint.hpp"
#include "const.hpp"

int CheckPoint::init(const char* devfile, bool read_mode) {
	// init member variables by Agent
	strcpy(this->devfile, devfile);
	this->fd = ::open(devfile, O_RDWR | O_NONBLOCK);
	if (fd < 0) {
		return 0; // handle error
	}
	// Set write|read pointer to the end of log section
	// lseek() requires "sys/types.h" & "unistd.h"
	off_t cond = lseek(fd, N_LOG_BLK * BLK_SIZE, SEEK_SET);
	if (cond == -1) {
		std::cout << "Error setting checkpoint writing offset: memory boundary passed.\n";
		return 0;
	}
	memset(this->buf, 0, sizeof this->buf);
	this->buf_tail = 0;
	if (read_mode) {
		this->buf_tail = BLK_SIZE;
	}
}

int CheckPoint::write(uint64_t val) {
	memcpy(buf + buf_tail, &val, 8);
	buf_tail += 8;
	if (buf_tail == BLK_SIZE) {
		// Write buffered blk to designated place of file descriptor
		ssize_t cond = ::write(fd, buf, BLK_SIZE);
		if (cond < 0) {
	    return 0; // handle real error
		}
		else {
			buf_tail = 0;
	    return 1;
		}
	} else {
		return 1; // assume no error in memcpy()
	}
}

int CheckPoint::read(uint64_t *ret, int len) {
	len *= 8;
	int ret_offset = 0;
	int left_bytes = BLK_SIZE - buf_tail;

 	while (len > left_bytes){
		memcpy(ret + ret_offset, buf + buf_tail, left_bytes);
		ret_offset += left_bytes / 8;
		len -= left_bytes;
		buf_tail = 0;
		ssize_t cond = ::read(fd, buf, BLK_SIZE);
		if (cond < 0) {
			return 0; // handle read error
		}
		left_bytes = BLK_SIZE;
	}
	// now there is enough buffered content for this read
	memcpy(ret + ret_offset, buf + buf_tail, len);
	buf_tail += len;
	return 1;
}

int CheckPoint::checkin() {
	// Write buffered blk to designated place of file descriptor
	ssize_t cond = ::write(fd, buf, buf_tail);  // buf_tail replaces BLK_SIZE
	if (cond < 0) {
		return 0; // handle real error
	} else {
		return 1;
	}
}

// return 1 for success, 0 for failure
// cache_length is in the form of uint64_t;
// write to device when opt is 1, else (==0) performs read
/*
int Graph::access_ckpoint(uint64_t cache[], int cache_length, int offset, int opt) {
	// Open the device in non-blocking mode
	int fd = open(DEVICE, O_RDWR | O_NONBLOCK);
	if (fd < 0) {
		return 0;  // handle error
	}
	// Set write|read pointer to the end of log section
	// lseek() requires "sys/types.h" & "unistd.h"
	off_t cond1 = lseek(fd, N_LOG_BLK * BLK_SIZE + offset, SEEK_SET);
	if (cond1 == -1) {
		cout << "Error setting checkpoint writing offset: memory boundary passed.\n";
		return 0;
	}

	// Write | Read byte string to designated place of file descriptor
	ssize_t cond2 = -1;
	if (opt) {
		cond2 = write(fd, cache, cache_length * 8);
	} else {
		cond2 = read(fd, cache, cache_length * 8);
	}
	close(fd);
	if (cond2 >= 0)
    return 1;  // handle successful write (which might be a partial write!)
	// else if (errno == EWOULDBLOCK)
  //   return 0;  // handle case where the write would block
	else
    return 0;  // handle real error
}
*/
