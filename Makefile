all : cs426_graph_server
cs426_graph_server: graph.cpp cs426_graph_server.cpp mongoose.c format.cpp
	g++ cs426_graph_server.cpp graph.cpp mongoose.c format.cpp -o cs426_graph_server 
