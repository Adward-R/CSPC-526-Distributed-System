#ifndef BACKUP_CLIENT_HPP
#define BACKUP_CLIENT_HPP

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include "gen-cpp/BackupService.h"

#define THRIFT_PORT 9091
using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

class BackupClient{
	public:
		boost::shared_ptr<TTransport> socket;
		boost::shared_ptr<TTransport> transport;
		boost::shared_ptr<TProtocol> protocol;
		boost::shared_ptr<BackupServiceClient> client;

		BackupClient();
		// return 1 on success, 0 otherwise
		int connect(char* ip);
		int add_node(uint64_t );
		int add_edge(uint64_t , uint64_t );
		int remove_node(uint64_t );
		int remove_edge(uint64_t , uint64_t );
};

#endif /* BACKUP_CLIENT_HPP */
