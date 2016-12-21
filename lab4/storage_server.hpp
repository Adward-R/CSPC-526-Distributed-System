#ifndef STORAGE_SERVER_HPP
#define STORAGE_SERVER_HPP

#include <string>
#include <pthread.h>
#include "graph.hpp"
#include "backup_client.hpp"

class StorageServer{
	public:
		Graph g;
		// clients to other partitions. partitionClient[i] is the client to the i-th partition
		BackupClient **partitionClient;
		int nPart;
		int partitionId;
		pthread_mutex_t mutex;

		StorageServer();
		//int connectBackupClient(char *ip, int port);
		int setupPartitions(int _partitionId, int _nPart, std::string *ip, int *port);
		int setPartitionId(int);
		int whichPartition(int);

		// These are updates, otherwise return whatever graph returns
		int add_node(uint64_t node_id);
		// return 1 on success, 0 if the edge exists, -1 if either node does not exist or a == b
		int add_edge(uint64_t a, uint64_t b);
		// [Deprecated] this will not be called
		int remove_node(uint64_t node_id); 
		// return 1 on success, -1 if the edge not exist
		int remove_edge(uint64_t a, uint64_t b);
		// return 1 on success, 0 if the node does not exist
		int get_node(uint64_t node_id);
		// return 1 on success, 0 if the edge does not exist, -1 if either node does not exist or a == b
		int get_edge(uint64_t a, uint64_t b);
		// return 1 on success, -1 if the node does not exist
		int get_neighbors(uint64_t node_id, std::vector<uint64_t> &res);
		// [Deprecated] this will not be called
		int shortest_path(uint64_t a, uint64_t b); 
};

#endif /* STORAGE_SERVER_HPP */
