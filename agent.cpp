#include <cstring>
#include "agent.hpp"

Agent::Agent(){
}

int Agent::open_devfile(char* _devfile){
	devfile = _devfile;
	disklog.open(devfile);
}

void Agent::format(){
	if (disklog.valid()){
		disklog.inc_gen();
	}else {
		disklog.reset();
	}
}

// read the checkpoint, and replay the logs
void Agent::start(){
	//TODO, read checkpoint
	//TODO, replay the logs
}

// return -3 for no log space, and otherwise the return values from the graph
int Agent::add_node(uint64_t node_id){
	if (!disklog.enough_space())
		return -3;
	int ret = g.add_node(node_id);
	if (ret == 1)
		disklog.add_entry(0, node_id, 0);
	return ret;
}

int Agent::add_edge(uint64_t a, uint64_t b){
	if (!disklog.enough_space())
		return -3;
	int ret = g.add_edge(a, b);
	if (ret == 1)
		disklog.add_entry(1, a, b);
	return ret;
}

int Agent::remove_node(uint64_t node_id){
	if (!disklog.enough_space())
		return -3;
	int ret = g.remove_node(node_id);
	if (ret == 1)
		disklog.add_entry(2, node_id, 0);
	return ret;
}

int Agent::remove_edge(uint64_t a, uint64_t b){
	if (!disklog.enough_space())
		return -3;
	int ret = g.remove_edge(a, b);
	if (ret == 1)
		disklog.add_entry(3, a, b);
	return ret;
}

// return the return values from the graph
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

// return 1 on success, -3 on no space
int Agent::checkpoint(){
	//TODO, call the graph's checkpoint
}
