#include <cstring>
#include <cstdio>
#include "agent.hpp"
#include "checkpoint.hpp"

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

// read the checkpoint, and replay the logs. return 0 on invalid superblock, 1 on valid
int Agent::start(){
	// if the disklog is invalid, return 0
	if (!disklog.valid()){
		return 0;
	}

	// read in-memory variables
	disklog.keep_variable_in_memory();

	//TODO, read checkpoint
	CheckPoint ckp;
	ckp.init(devfile.c_str(), true);
	g.restore(ckp);

	// replay the logs
	for (Disklog::iterator it = disklog.begin(); !it.end(); it.next()){
		Logentry* entry = it.entry();
		switch (entry->opcode){
			case 0:
				g.add_node(entry->a);
				break;
			case 1:
				g.add_edge(entry->a, entry->b);
				break;
			case 2:
				g.remove_node(entry->a);
				break;
			case 3:
				g.remove_edge(entry->a, entry->b);
				break;
			default:
				printf("Agent::start(): undefined opcode\n");
				break;
		}
	}
	return 1;
}

// return -3 for no log space, and otherwise the return values from the graph
int Agent::add_node(uint64_t node_id){
	if (!disklog.enough_space()){
		return -3;
	}
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
	CheckPoint ckp;
	ckp.init(devfile.c_str(), false);
	if (g.checkpoint(ckp))
		return 1;
	else 
		return -3;
}
