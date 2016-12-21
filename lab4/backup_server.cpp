#include "backup_server.hpp"

BackupServer::BackupServer(StorageServer *_storageServer) {
	storageServer = _storageServer;
}

int64_t BackupServer::backup(const Operation::type op, const int64_t param1, const int64_t param2){
	int ret = 0;
	switch (op) {
		case Operation::ADD_NODE:
			ret = storageServer->add_node((uint64_t) param1);
			break;
		case Operation::REMOVE_NODE:
			ret = storageServer->remove_node((uint64_t) param1);
			break;
		case Operation::ADD_EDGE:
			ret = storageServer->add_edge((uint64_t) param1, (uint64_t) param2);
			break;
		case Operation::REMOVE_EDGE:
			ret = storageServer->remove_edge((uint64_t) param1, (uint64_t) param2);
			break;
		case Operator::GET_NODE:
			ret = storageServer->get_node((uint64_t) param1);
			break;
		default:
			break;
	}
	return ret;
}
