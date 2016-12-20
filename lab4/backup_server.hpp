#ifndef BACKUP_SERVER_HPP
#define BACKUP_SERVER_HPP

#include "gen-cpp/backup_interface_constants.h"
#include "gen-cpp/backup_interface_types.h"
#include "gen-cpp/BackupService.h"
#include "storage_server.hpp"

using namespace std;
class BackupServer: public BackupServiceIf {
	public:
		StorageServer *storageServer;

		BackupServer(StorageServer *_storageServer);
		virtual int64_t backup(const Operation::type op, const int64_t param1, const int64_t param2);
};

#if 0
class GraphServiceCloneFactory : virtual public GraphServiceIfFactory {
	public:
		virtual ~GraphServiceCloneFactory() {}
		virtual GraphServiceIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo)
		{
			boost::shared_ptr<TSocket> sock = boost::dynamic_pointer_cast<TSocket>(connInfo.transport);
			// cout << "Incoming connection\n";
			// cout << "\tSocketInfo: "  << sock->getSocketInfo() << "\n";
			// cout << "\tPeerHost: "    << sock->getPeerHost() << "\n";
			// cout << "\tPeerAddress: " << sock->getPeerAddress() << "\n";
			// cout << "\tPeerPort: "    << sock->getPeerPort() << "\n";
			return new GraphServiceHandler;
		}
		virtual void releaseHandler( ::GraphServiceIf* handler) {
			delete handler;
		}
};
#endif

#endif /* BACKUP_SERVER_HPP */

