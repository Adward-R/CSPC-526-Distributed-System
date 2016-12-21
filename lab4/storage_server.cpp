#include <algorithm>
#include "storage_server.hpp"
using namespace std;

StorageServer::StorageServer() {
	pthread_mutex_init(&mutex, NULL);
}

int StorageServer::setupPartitions(int _partitionId, int _nPart, string *ip, int *port){
	partitionId = _partitionId;
	nPart = _nPart;
	partitionClient = new BackupClient*[nPart];
	for (int i = 0; i < nPart; i++){
		if (i == partitionId)
			continue;
		partitionClient[i] = new BackupClient();
		int ret = partitionClient[i]->connect(ip[i].c_str(), port[i]);
		if (ret != 1)
			return 0;
	}
	return 1;
}

int StorageServer::setPartitionId(int _partitionId){
	partitionId = _partitionId;
}

int StorageServer::whichPartition(int node){
	return node % nPart;
}

int StorageServer::add_node(uint64_t node_id){
	pthread_mutex_lock(&mutex);
	int ret = g.add_node(node_id);
	pthread_mutex_unlock(&mutex);
	return ret;
}

// ordering if a and b are at diff partitions:
// part a		part b
// lock a
// a != b
// a exist
// 				lock b
// 				b exist
// 				add b->a
//				unlock b
// add a->b
// unlock a
int StorageServer::add_edge(uint64_t a, uint64_t b){
	pthread_mutex_lock(&mutex);
	int ret = 1;
	do {
		// a==b, invalid
		if (a == b){
			ret = -1;
			break;
		}
		// first check partitions of a and b
		int pa = whichPartition(a), pb = whichPartition(b);
		// put the one with lower partition # at a
		if (pa > pb){
			swap(a, b);
			swap(pa, pb);
		}

		// if a belongs to this partition, this is the main partition, check a existence.
		if (pa == partitionId){
			// if a not exists, return -1
			if (g.get_node(a) == 0){
				ret = -1;
				break;
			}
			// if b belong to this partition
			if (pb == partitionId){
				// if b not exists, return -1
				if (g.get_node(b) == 0){
					ret = -1;
					break;
				}
				// try add b->a
				// if edge exists, return 0
				if (g.add_edge(b, a) == 0){ 
					ret = 0;
					break;
				}
			} else { // otherwise, b belong to another (largar) partition, send to it
				// the other partition will check b's existence, and try add b->a
				ret = partitionClient[pb]->add_edge(b, a);
				// if the other partition fails (b not exist, or b->a exists)
				if (ret != 1) 
					break;
			}
			// b->a succeeds, add a->b as well
			ret = g.add_edge(a, b);
		} else { // otherwise, this update is from the main partition
			// b should = partitionId
			if (b != partitionId){
				printf("[StorageServer::add_edge] both nodes does not belong to this partition\n");
				ret = -100;
				break;
			}
			// if b not exists
			if (g.get_node(b) == 0){
				ret = -1;
				break;
			}
			ret = g.add_edge(b, a);
		}
	} while (0);
	pthread_mutex_unlock(&mutex);
	return ret;
}

// this will not be called
int StorageServer::remove_node(uint64_t node_id){
	pthread_mutex_lock(&mutex);
	int ret = g.remove_node(node_id);
	pthread_mutex_unlock(&mutex);
	return ret;
}

// ordering if a and b are at diff partitions:
// part a		part b
// lock a
// a exist
// 				lock b
// 				b exist
// 				b->a exist
// 				remove b->a
// 				unlock b
// remove a->b (if part b OK)
// unlock a
int StorageServer::remove_edge(uint64_t a, uint64_t b){
	pthread_mutex_lock(&mutex);
	int ret = 1;
	do {
		if (a == b){
			ret = -1;
			break;
		}
		// first check partitions of a and b
		int pa = whichPartition(a), pb = whichPartition(b);
		// put the one with lower partition # at a
		if (pa > pb){
			swap(a, b);
			swap(pa, pb);
		}
		// if this is the main partition
		if (pa == partitionId){
			// if a not exists, return -1
			if (g.get_node(a) == 0){
				ret = -1;
				break;
			}
			// if b belongs to this partition
			if (pb == partitionId){
				// if b not exists, return -1
				if (g.get_node(b) == 0){
					ret = -1;
					break;
				}
				// try remove b->a
				// if b->a not exists, return -1
				if (g.remove_edge(b, a) == -1){
					ret = -1;
					break;
				}
			}else { // otherwise, b belong to another (largar) partition, send to it
				ret = partitionClient[pb]->remove_edge(b, a);
				// if the other partition fails (b not exist, or b->a not exists)
				if (ret != 1)
					break;
			}
			ret = g.remove_edge(a, b);
		} else { // otherwise, this update is from the main partition
			// b should = partitionId
			if (b != partitionId){
				printf("[StorageServer::remove_edge] both nodes does not belong to this partition\n");
				ret = -100;
				break;
			}
			// if b not exists
			if (g.get_node(b) == 0){
				ret = -1;
				break;
			}
			ret = g.remove_edge(b, a);
		}
	} while (0);
	pthread_mutex_unlock(&mutex);
	return ret;
}

int StorageServer::get_node(uint64_t node_id){
	pthread_mutex_lock(&mutex);
	int ret = g.get_node(node_id);
	pthread_mutex_unlock(&mutex);
	return ret;
}

// ordering if a and b are at diff partitions:
// part a		part b
// lock a
// a exist
// 				lock b
// 				b exist
// 				b->a exist
// 				unlock b
// unlock a
int StorageServer::get_edge(uint64_t a, uint64_t b){
	pthread_mutex_lock(&mutex);
	int ret = 1;
	do {
		if (a == b){
			ret = -1;
			break;
		}
		// first check partitions of a and b
		int pa = whichPartition(a), pb = whichPartition(b);
		// put the one with lower partition # at a
		if (pa > pb){
			swap(a, b);
			swap(pa, pb);
		}
		// if this is the main partition
		if (pa == partitionId){
			// if a not exists, return -1
			if (g.get_node(a) == 0){
				ret = -1;
				break;
			}
			// if b belongs to this partition
			if (pb == partitionId){
				// if b not exists, return -1
				if (g.get_node(b) == 0){
					ret = -1;
					break;
				}
				ret = g.get_edge(b, a);
			}else { // otherwise, b belong to another (largar) partition, send to it
				ret = partitionClient[pb]->get_edge(b, a);
			}
		} else { // otherwise, this update is from the main partition
			// b should = partitionId
			if (b != partitionId){
				printf("[StorageServer::get_edge] both nodes does not belong to this partition\n");
				ret = -100;
				break;
			}
			// if b not exists
			if (g.get_node(b) == 0){
				ret = -1;
				break;
			}
			ret = g.get_edge(b, a);
		}
	} while (0);
	pthread_mutex_unlock(&mutex);
	return ret;
}

int StorageServer::get_neighbors(uint64_t node_id, std::vector<uint64_t> &res){
	pthread_mutex_lock(&mutex);
	int ret = g.get_neighbors(node_id, res);
	pthread_mutex_unlock(&mutex);
	return ret;
}

// this will not be called
int StorageServer::shortest_path(uint64_t a, uint64_t b){
	pthread_mutex_lock(&mutex);
	int ret = g.shortest_path(a,b);
	pthread_mutex_unlock(&mutex);
	return ret;
}
