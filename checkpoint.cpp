#include "const.hpp"

// return 1 for success, 0 for failure
// cache_length is in the form of uint64_t;
// write to device when opt is 1, else (==0) performs read
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

// return 1 on success, 0 on failure for present
int Graph::checkpoint() {
	map<uint64_t,set<uint64_t> >::iterator it;
	set<uint64_t>::iterator its;

	// Get the volume in words (8 byte) of current graph after serializing
	int volume = 2;  // already included <volume> itself & <num of nodes>
	for (it = nodes.begin(); it != nodes.end(); it++) {
		volume += (it->second).size();
	}
	// Create the array space for graph serialization
	uint64_t ckpoint[volume];

	// Serialize the in-memory graph map into a byte string
	ckpoint[0] = volume;
	ckpoint[1] = (uint64_t) nodes.size();
	int tail = 1;
	for (it = nodes.begin(); it != nodes.end(); it++) {
		ckpoint[tail++] = it->first;
		ckpoint[tail++] = (uint64_t) (it->second).size();
		for (its = (it->second).begin(); its != (it->second).end(); its++) {
			ckpoint[tail++] = *its;
		}
	}

	return access_ckpoint(ckpoint, tail, 0, 1); // 1 for write
}

// return 1 for success, 0 for failure
int Graph::restore() {
	uint64_t volume;
	if (access_ckpoint(volume, 8, 0, 0) == 0) {
		return 0;
	} // 0 for read

	uint64_t ckpoint[volume-1];
	access_ckpoint(ckpoint, volume-1, 8, 0);
	uint64_t n_nodes = ckpoint[0];

	int i = 1, i_node = 0;
	while (i_node < n_nodes) {
		uint64_t key = ckpoint[i];
		uint64_t n_nbrs = ckpoint[i+1];
		nodes[key];
		i += 2;
		for (int j = 0; j < n_nbrs; j++) {
			nodes[key].insert(ckpoint[i++]);
		}
		i_node++;
	}
	return 1;
}

