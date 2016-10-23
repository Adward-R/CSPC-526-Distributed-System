#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#include <cstdio>
#include <cstring>

#include "const.hpp"

char buf[BLK_SIZE] __attribute__ ((__aligned__ (BLK_SIZE)));

int test_superblock(int fd){
	char *m = (char*)mmap(0, BLK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	if (m == MAP_FAILED){
		perror("mmap() in test_superblock()");
		return -1;
	}
	Superblock *sb = (Superblock*)m;
	int ret = sb->is_valid();
	munmap(m, BLK_SIZE);
	return ret;
}

// format the devfile. return -1 for failure, 0 for not format, 1 for format
int format(char *devfile){
	// open the disk
	int ret = 0;
	int fd = open(devfile, O_RDWR);
	if (fd == -1){
		perror("open() in format()");
		return ;
	}

	// test superblock
	ret = test_superblock(fd);

	// failure
	if (ret == -1){
		close(fd);
		return -1;
	}

	// no need for format
	if (ret == 1){
		close(fd);
		return 0;
	}

#if 0
	// set the buffer to 0, except the first to a magic number
	memset(buf, 0, sizeof(buf));
	((int64_t*)buf)[0] = MAGIC_NUMBER;

	// write the buffer (0) to the disk
	int ret;
	for (int i = 0; i < N_BLK; i++){
		ret = write(fd, buf, BLK_SIZE);
		if (ret == -1){
			perror("write() in format()");
			return;
		}
	}
#endif

	// close the device
	close(fd);
	
	return 1;
}
