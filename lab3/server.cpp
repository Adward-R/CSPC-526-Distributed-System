#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/PlatformThreadFactory.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/TToString.h>

#include <boost/make_shared.hpp>

#include <iostream>
#include <stdexcept>
#include <sstream>

#include "gen-cpp/graph_constants.h"
#include "gen-cpp/graph_types.h"
#include "graph.hpp"

using namespace std;
using namespace apache::thrift;
using namespace apache::thrift::concurrency;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;
using namespace apache::thrift::server;

class GraphServiceHandler : public GraphServiceIf {
protected:
  Graph g; // not sure if should be defined here
public:
  GraphServiceHandler() {}

  int64_t mutate(const Graph_ops& graph_ops) {
    int ret = 0;
    switch (graph_ops.op) {
      case Operation::ADD_NODE:
        // paste graph.cpp code here
        ret = g.add_node((uint64_t) graph_ops.param1);
        break;
      case Operation::REMOVE_NODE:
        // paste graph.cpp code here
        ret = g.remove_node((uint64_t) graph_ops.param1);
        break;
      case Operation::ADD_EDGE:
        // paste graph.cpp code here
        ret = g.add_edge((uint64_t) graph_ops.param1, (uint64_t) graph_ops.param2);
        break;
      case Operation::REMOVE_EDGE:
        // paste graph.cpp code here
        ret = g.remove_edge((uint64_t) graph_ops.param1, (uint64_t) graph_ops.param2);
        break;
      default:;
    }
    // if ret is irregular, throw exceptions or return 500 sth
    // dispose here
  }

};

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

/*
int main() {
  TThreadedServer server(
    boost::make_shared<CalculatorProcessorFactory>(boost::make_shared<CalculatorCloneFactory>()),
    boost::make_shared<TServerSocket>(9090), //port
    boost::make_shared<TBufferedTransportFactory>(),
    boost::make_shared<TBinaryProtocolFactory>());


  // if you don't need per-connection state, do the following instead
  TThreadedServer server(
    boost::make_shared<CalculatorProcessor>(boost::make_shared<CalculatorHandler>()),
    boost::make_shared<TServerSocket>(9090), //port
    boost::make_shared<TBufferedTransportFactory>(),
    boost::make_shared<TBinaryProtocolFactory>());


  // Here are some alternate server types...

*/
