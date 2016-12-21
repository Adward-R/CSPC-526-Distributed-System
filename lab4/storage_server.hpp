#ifndef STORAGE_SERVER_HPP
#define STORAGE_SERVER_HPP

#include <string>
#include "graph.hpp"
#include "backup_client.hpp"

class StorageServer{
	public:
		Graph g;
		// clients to other partitions. partitionClient[i] is the client to the i-th partition
		BackupClient **partitionClient;
		int nPart;
		int partitionId;

		StorageServer();
		//int connectBackupClient(char *ip, int port);
		int setupPartitions(int _partitionId, int _nPart, std::string *ip, int *port);
		int setPartitionId(int);
		static int whichPartition(int);

		// These are updates, return -100 if backup fails, otherwise return whatever graph returns
		int add_node(uint64_t node_id);
		int add_edge(uint64_t a, uint64_t b);
		int remove_node(uint64_t node_id); // this will not be called
		int remove_edge(uint64_t a, uint64_t b);
		// These are queries, just call graph and return whatever graph returns
		int get_node(uint64_t node_id);
		int get_edge(uint64_t a, uint64_t b);
		int get_neighbors(uint64_t node_id, std::vector<uint64_t> &res);
		int shortest_path(uint64_t a, uint64_t b);
};

#endif /* STORAGE_SERVER_HPP */
