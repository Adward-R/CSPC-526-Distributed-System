#ifndef STORAGE_SERVER_HPP
#define STORAGE_SERVER_HPP

#include "graph.hpp"
#include "backup_client.hpp"

class StorageServer{
	public:
		Graph g;
		BackupClient *backupClient;

		StorageServer();
		int connectBackupClient(char *ip);

		// These are updates, return -100 if backup fails, otherwise return whatever graph returns
		int add_node(uint64_t node_id);
		int add_edge(uint64_t a, uint64_t b);
		int remove_node(uint64_t node_id);
		int remove_edge(uint64_t a, uint64_t b);
		// These are queries, just call graph and return whatever graph returns
		int get_node(uint64_t node_id);
		int get_edge(uint64_t a, uint64_t b);
		int get_neighbors(uint64_t node_id, std::vector<uint64_t> &res);
		int shortest_path(uint64_t a, uint64_t b);
};

#endif /* STORAGE_SERVER_HPP */
