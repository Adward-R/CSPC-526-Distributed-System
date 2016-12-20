#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <map>
#include <set>
#include <stdint.h>

class Graph{
	private:
		std::map<uint64_t,std::set<uint64_t> > nodes;
	public:
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
};
#endif /* GRAPH_HPP */
