#include "storage_server.hpp"
using namespace std;

StorageServer::StorageServer() {
}

#if 0
int StorageServer::connectBackupClient(char *ip, int port){
	backupClient = new BackupClient();
	int ret = backupClient->connect(ip, port);
	return ret;
}
#endif

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
	return node % 3;
}

int StorageServer::add_node(uint64_t node_id){
	return g.add_node(node_id);
}

int StorageServer::add_edge(uint64_t a, uint64_t b){
	int ret = 1;

	if (ret == 1) // if successfully backup
		ret = g.add_edge(a,b);
	else // otherwise
		ret = -100;

	return ret;
}

// this will not be called
int StorageServer::remove_node(uint64_t node_id){
	return g.remove_node(node_id);
}

int StorageServer::remove_edge(uint64_t a, uint64_t b){
	int ret = 1;

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
