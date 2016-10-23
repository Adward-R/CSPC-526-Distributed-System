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
		~Agent();

		// open the log device
		int open_devfile(char* _devfile);

		// format
		void format();

		// read the checkpoint, and replay the logs
		int start();

		// return -3 for no log space, and otherwise the return values from the graph
		int add_node(uint64_t node_id);
		int add_edge(uint64_t a, uint64_t b);
		int remove_node(uint64_t node_id);
		int remove_edge(uint64_t a, uint64_t b);

		// return the return values from the graph
		int get_node(uint64_t node_id);
		int get_edge(uint64_t a, uint64_t b);
		int get_neighbors(uint64_t node_id, std::vector<uint64_t> &res);
		int shortest_path(uint64_t a, uint64_t b);

		// return 1 on success, -3 on no space
		int checkpoint();
};

#endif /* AGENT_HPP */
