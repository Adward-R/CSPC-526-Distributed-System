#include "graph.hpp"
using namespace std;

// Define global settings
const int BLOCK_SIZE = 4 * 1024; // 4k bytes
const int LOG_BLOCK_N = 0.5 * 1024 * 1024; // 0.5M
const char DEVICE[] = "/dev/sda/";

Graph::Graph(){
}

// return 1 on success, 0 if the node exists
int Graph::add_node(uint64_t node_id){
	if (nodes.find(node_id) == nodes.end()){
		nodes[node_id];
		return 1;
	}
	return 0;
}

// return 1 on success, 0 if the edge exists, -1 if either node does not exist or a == b
int Graph::add_edge(uint64_t a, uint64_t b){
	if (a==b || nodes.find(a) == nodes.end() || nodes.find(b) == nodes.end())
		return -1;

	// if a does not link with b, insert the edge
	if (nodes[a].find(b)==nodes[a].end()){
		nodes[a].insert(b);
		nodes[b].insert(a);
		return 1;
	}
	return 0;
}

// return 1 on success, -1 if the node does not exist
int Graph::remove_node(uint64_t node_id){
	if (nodes.find(node_id) == nodes.end())
		return -1;

	// remove node_id from its neighbor's neighbor list
	set<uint64_t> &neighb = nodes[node_id];
	for (set<uint64_t>::iterator it = neighb.begin(); it != neighb.end(); it++){
		uint64_t b = *it;
		nodes[b].erase(node_id);
	}

	//remove node_id
	nodes.erase(node_id);
	return 1;
}

// return 1 on success, -1 if the edge not exist
int Graph::remove_edge(uint64_t a, uint64_t b){
	if (nodes.find(a) == nodes.end() || nodes.find(b) == nodes.end())
		return -1;
	if (nodes[a].find(b) == nodes[a].end())
		return -1;

	nodes[a].erase(b);
	nodes[b].erase(a);
	return 1;
}

// return 1 on success, 0 if the node does not exist
int Graph::get_node(uint64_t node_id){
	if (nodes.find(node_id) == nodes.end())
		return 0;
	return 1;
}

// return 1 on success, 0 if the edge does not exist, -1 if either node does not exist
int Graph::get_edge(uint64_t a, uint64_t b){
	if (a==b || nodes.find(a) == nodes.end() || nodes.find(b) == nodes.end())
		return -1;
	if (nodes[a].find(b) == nodes[a].end())
		return 0;
	return 1;
}

// return 1 on success, -1 if the node does not exist
int Graph::get_neighbors(uint64_t node_id, vector<uint64_t>& res){
	if (nodes.find(node_id) == nodes.end())
		return -1;
	res.assign(nodes[node_id].begin(), nodes[node_id].end());
	return 1;
}

// return <distance> on success, -1 if no path, -2 if either nodes does not exist
int Graph::shortest_path(uint64_t a, uint64_t b){
	if (nodes.find(a) == nodes.end() || nodes.find(b) == nodes.end())
		return -2;
	vector<uint64_t> q;
	int head;
	map<uint64_t, int> dis;
	q.push_back(a);
	dis[a] = 0;
	for (int head = 0; head < q.size(); head++){
		uint64_t x = q[head];
		for (set<uint64_t>::iterator it = nodes[x].begin(); it != nodes[x].end(); it++){
			uint64_t y = *it;
			if (dis.find(y) == dis.end()){
				q.push_back(y);
				dis[y] = dis[x] + 1;
				if (y == b)
					return dis[y];
			}
		}
	}
	return -1;
}

// return 1 for success, 0 for failure
// cache_length is in the form of uint64_t;
// write to device when opt is 1, else (==0) performs read
int access_ckpoint(uint64_t cache[], int cache_length, int offset, int opt) {
	// Open the device in non-blocking mode
	int fd = open(DEVICE, O_RDWR | O_NONBLOCK);
	if (fd < 0) {
		return 0;  // handle error
	}
	// Set write|read pointer to the end of log section
	// lseek() requires "sys/types.h" & "unistd.h"
	off_t cond1 = lseek(fd, LOG_BLOCK_N * BLOCK_SIZE + offset, SEEK_SET);
	if (cond1 == -1) {
		cout << "Error setting checkpoint writing offset: memory boundary passed.\n"
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
