#include <fcntl.h>
#include <sys/mman.h>
#include <cstdio>
#include <unistd.h>
#include <stdint.h>
#include "const.hpp"

int main(){
	int fd = open("/dev/sdb", O_RDWR);
	if (fd == -1){
		perror("open");
		return 0;
	}

	off_t cond = lseek(fd, N_LOG_BLK * BLK_SIZE, SEEK_SET);
	if (cond == -1){
		perror("lseek");
		return 0;
	}

	uint64_t *mem = (uint64_t*)mmap(0, BLK_SIZE * 2, PROT_READ | PROT_WRITE, MAP_SHARED, fd, N_LOG_BLK * BLK_SIZE);
	if (mem == MAP_FAILED){
		perror("mmap");
		return 0;
	}

	for (int i = 0; i < 100; i++){
		printf("%llu\n", mem[i]);
	}
	return 0;
}
