#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <map>
#include <set>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include "checkpoint.hpp"

#include "checkpoint.hpp"

class Graph{
	public:
		std::map<uint64_t,std::set<uint64_t> > nodes;
		Graph();
		// return 1 on success, 0 if the node exists
		int add_node(uint64_t node_id);
		int add_edge(uint64_t a, uint64_t b);
		int remove_node(uint64_t node_id);
		int remove_edge(uint64_t a, uint64_t b);
		int get_node(uint64_t node_id);
		int get_edge(uint64_t a, uint64_t b);
		int get_neighbors(uint64_t node_id, std::vector<uint64_t> &res);
		int shortest_path(uint64_t a, uint64_t b);
		int checkpoint(CheckPoint& ckpoint);
		int restore(CheckPoint& ckpoint);
};
#endif /* GRAPH_HPP */
