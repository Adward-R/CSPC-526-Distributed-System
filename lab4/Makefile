all : cs426_graph_server
cs426_graph_server: cs426_graph_server.cpp storage_server.cpp graph.cpp backup_client.cpp backup_server.cpp mongoose.c
	g++ cs426_graph_server.cpp storage_server.cpp graph.cpp backup_client.cpp backup_server.cpp gen-cpp/BackupService.cpp gen-cpp/backup_interface_constants.cpp gen-cpp/backup_interface_types.cpp mongoose.c -o cs426_graph_server -L/usr/local/lib/ -lthrift -lpthread
