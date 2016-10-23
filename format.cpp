#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>

#include "const.hpp"

char buf[BLK_SIZE] __attribute__ ((__aligned__ (BLK_SIZE)));

// format n blocks
void format(int n){
	// open the disk
	int fd = open(DEVICE, O_RDWR);
	if (fd == -1){
		perror("open");
		return ;
	}

	// set the buffer to 0, except the first to a magic number
	memset(buf, 0, sizeof(buf));
	((int64_t*)buf)[0] = MAGIC_NUMBER;

	// write the buffer (0) to the disk
	int ret;
	for (int i = 0; i < n; i++){
		ret = write(fd, buf, BLK_SIZE);
		if (ret == -1){
			perror("write");
			return;
		}
	}
}
