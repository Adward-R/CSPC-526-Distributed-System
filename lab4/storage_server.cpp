#include "storage_server.hpp"

StorageServer::StorageServer() {
	backupClient = 0;
}

int StorageServer::connectBackupClient(char *ip){
	backupClient = new BackupClient();
	int ret = backupClient->connect(ip);
	return ret;
}

int StorageServer::add_node(uint64_t node_id){
	int ret = 1;
	// if have backup, first backup
	if (backupClient != 0)
		ret = backupClient->add_node(node_id);

	if (ret == 1) // if successfully backup
		ret = g.add_node(node_id);
	else // otherwise
		ret = -100;

	return ret;
}

int StorageServer::add_edge(uint64_t a, uint64_t b){
	int ret = 1;
	// if have backup, first backup
	if (backupClient != 0)
		ret = backupClient->add_edge(a,b);

	if (ret == 1) // if successfully backup
		ret = g.add_edge(a,b);
	else // otherwise
		ret = -100;

	return ret;
}

int StorageServer::remove_node(uint64_t node_id){
	int ret = 1;
	// if have backup, first backup
	if (backupClient != 0)
		ret = backupClient->remove_node(node_id);

	if (ret == 1) // if successfully backup
		ret = g.remove_node(node_id);
	else // otherwise
		ret = -100;

	return ret;
}
int StorageServer::remove_edge(uint64_t a, uint64_t b){
	int ret = 1;
	// if have backup, first backup
	if (backupClient != 0)
		ret = backupClient->remove_edge(a,b);

	if (ret == 1) // if successfully backup
		ret = g.remove_edge(a,b);
	else // otherwise
		ret = -100;

	return ret;
}

int StorageServer::get_node(uint64_t node_id){
	return g.get_node(node_id);
}

int StorageServer::get_edge(uint64_t a, uint64_t b){
	return g.get_edge(a,b);
}

int StorageServer::get_neighbors(uint64_t node_id, std::vector<uint64_t> &res){
	return g.get_neighbors(node_id, res);
}

int StorageServer::shortest_path(uint64_t a, uint64_t b){
	return g.shortest_path(a,b);
}
