#include "backup_client.hpp"

BackupClient::BackupClient(){
}
int BackupClient::connect(char* backupIp){
	socket = boost::shared_ptr<TTransport> (new TSocket(backupIp, THRIFT_PORT));
	transport = boost::shared_ptr<TTransport> (new TBufferedTransport(socket));
	protocol = boost::shared_ptr<TProtocol> (new TBinaryProtocol(transport));
	client = boost::shared_ptr<BackupServiceClient> (new BackupServiceClient(protocol));

	try{
		printf("[BackupClient::connect] Connecting to backup server %s\n", backupIp);
		transport->open();
	} catch (TException& tx){
		printf("%s\n", tx.what());
		return 0;
	}
	printf("[BackupClient::connect] Successfully connected to backup server %s\n", backupIp);
	return 1;
}

int BackupClient::add_node(uint64_t _node){
	if (_node > 0x7fffffffffffffff)
		printf("[BackupClient::add_node] Warning: node id exceeds int64_t range\n");
	int64_t node = (int64_t) _node;
	client->backup(Operation::ADD_NODE, node, -1);
	return 1;
}

int BackupClient::add_edge(uint64_t _a, uint64_t _b){
	if (_a > 0x7fffffffffffffff || _b > 0x7fffffffffffffff)
		printf("[BackupClient::add_edge] Warning: node id exceeds int64_t range\n");
	int64_t a = (int64_t)_a, b = (int64_t)_b;
	client->backup(Operation::ADD_EDGE, a, b);
	return 1;
}

int BackupClient::remove_node(uint64_t _node){
	if (_node > 0x7fffffffffffffff)
		printf("[BackupClient::remove_node] Warning: node id exceeds int64_t range\n");
	int64_t node = (int64_t) _node;
	client->backup(Operation::REMOVE_NODE, node, -1);
	return 1;
}

int BackupClient::remove_edge(uint64_t _a, uint64_t _b){
	if (_a > 0x7fffffffffffffff || _b > 0x7fffffffffffffff)
		printf("[BackupClient::remove_edge] Warning: node id exceeds int64_t range\n");
	int64_t a = (int64_t)_a, b = (int64_t)_b;
	client->backup(Operation::REMOVE_EDGE, a, b);
	return 1;
}
