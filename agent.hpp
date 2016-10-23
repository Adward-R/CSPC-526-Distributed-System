#ifndef AGENT_HPP
#define AGENT_HPP

#include <string>

#include "graph.hpp"
#include "disklog.hpp"

class Agent{
	private:
		Graph g;
		Disklog disklog;
		std::string devfile;
	public:
		Agent();
		int add_devfile(char* _devfile);
		// format. return -1 for failure, 0 for not format, 1 for format
		int format();

		int add_node(uint64_t node_id);
		int add_edge(uint64_t a, uint64_t b);
		int remove_node(uint64_t node_id);
		int remove_edge(uint64_t a, uint64_t b);
		int get_node(uint64_t node_id);
		int get_edge(uint64_t a, uint64_t b);
		int get_neighbors(uint64_t node_id, std::vector<uint64_t> &res);
		int shortest_path(uint64_t a, uint64_t b);
};

#endif /* AGENT_HPP */
