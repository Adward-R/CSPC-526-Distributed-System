all : cs426_graph_server
cs426_graph_server: graph.cpp cs426_graph_server.cpp mongoose.c backup_client.cpp
	g++ cs426_graph_server.cpp graph.cpp backup_client.cpp gen-cpp/BackupService.cpp gen-cpp/backup_interface_constants.cpp gen-cpp/backup_interface_types.cpp mongoose.c -o cs426_graph_server -L/usr/local/lib/ -lthrift
