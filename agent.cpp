#include <cstring>
#include "agent.hpp"

Agent::Agent(){
}

int Agent::add_devfile(char* _devfile){
	devfile = _devfile;
}

// format. return -1 for failure, 0 for not format, 1 for format
int Agent::format(){
	// TODO 
	return 0;
}

int Agent::add_node(uint64_t node_id){
	int ret = g.add_node(node_id);
	// TODO update log
	return ret;
}

int Agent::add_edge(uint64_t a, uint64_t b){
	int ret = g.add_edge(a, b);
	// TODO update log
	return ret;
}

int Agent::remove_node(uint64_t node_id){
	int ret = g.remove_node(node_id);
	// TODO update log
	return ret;
}

int Agent::remove_edge(uint64_t a, uint64_t b){
	int ret = g.remove_edge(a, b);
	// TODO update log
	return ret;
}

int Agent::get_node(uint64_t node_id){
	int ret = g.get_node(node_id);
	return ret;
}

int Agent::get_edge(uint64_t a, uint64_t b){
	int ret = g.get_edge(a, b);
	return ret;
}

int Agent::get_neighbors(uint64_t node_id, std::vector<uint64_t> &res){
	int ret = g.get_neighbors(node_id, res);
	return ret;
}

int Agent::shortest_path(uint64_t a, uint64_t b){
	int ret = g.shortest_path(a, b);
	return ret;
}

