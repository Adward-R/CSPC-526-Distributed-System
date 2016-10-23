all : cs426_graph_server
cs426_graph_server: agent.cpp graph.cpp disklog.cpp cs426_graph_server.cpp mongoose.c superblock.cpp
	g++ cs426_graph_server.cpp agent.cpp graph.cpp disklog.cpp mongoose.c superblock.cpp -o cs426_graph_server 
