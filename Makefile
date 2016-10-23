all : cs426_graph_server
cs426_graph_server: cs426_graph_server.cpp agent.cpp graph.cpp disklog.cpp superblock.cpp logblock.cpp checkpoint.cpp mongoose.c 
	g++ cs426_graph_server.cpp agent.cpp graph.cpp disklog.cpp superblock.cpp logblock.cpp checkpoint.cpp mongoose.c -o cs426_graph_server 

test: test.cpp logblock.cpp 
	g++ test.cpp logblock.cpp -o test
