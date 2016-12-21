# CSPC-526-Distributed-System
----
By Yuliang Li & Keyang Dong

# Lab 4 Work-split

Design the locking scheme together

## Yuliang Li

- Refactor the storage_server.cpp/hpp to lock and RPC other partitoins
- Refactor the cs426_graph_server.cpp to support try connect to other partition until success

## Keyang Dong

- Refactor the thrift and BackupClient/BackupServer to support new thrift interface get_node, used for checking node existence on other partitions
- Change the graph.cpp/hpp (remove the node existence checking from add_edge, remove_edge, and get_edge, which will be done in storage_server)
