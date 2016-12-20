#include "graph.hpp"
using namespace std;

Graph::Graph(){
}

// return 1 on success, 0 if the node exists
int Graph::add_node(uint64_t node_id){
	if (nodes.find(node_id) == nodes.end()){
		nodes[node_id];
		return 1;
	}
	return 0;
}

// return 1 on success, 0 if the edge exists, -1 if either node does not exist or a == b
int Graph::add_edge(uint64_t a, uint64_t b){
	if (a==b || nodes.find(a) == nodes.end() || nodes.find(b) == nodes.end())
		return -1;

	// if a does not link with b, insert the edge
	if (nodes[a].find(b)==nodes[a].end()){
		nodes[a].insert(b);
		nodes[b].insert(a);
		return 1;
	}
	return 0;
}

// return 1 on success, -1 if the node does not exist
int Graph::remove_node(uint64_t node_id){
	if (nodes.find(node_id) == nodes.end())
		return -1;

	// remove node_id from its neighbor's neighbor list
	set<uint64_t> &neighb = nodes[node_id];
	for (set<uint64_t>::iterator it = neighb.begin(); it != neighb.end(); it++){
		uint64_t b = *it;
		nodes[b].erase(node_id);
	}

	//remove node_id
	nodes.erase(node_id);
	return 1;
}

// return 1 on success, -1 if the edge not exist
int Graph::remove_edge(uint64_t a, uint64_t b){
	if (nodes.find(a) == nodes.end() || nodes.find(b) == nodes.end())
		return -1;
	if (nodes[a].find(b) == nodes[a].end())
		return -1;

	nodes[a].erase(b);
	nodes[b].erase(a);
	return 1;
}

// return 1 on success, 0 if the node does not exist
int Graph::get_node(uint64_t node_id){
	if (nodes.find(node_id) == nodes.end())
		return 0;
	return 1;
}

// return 1 on success, 0 if the edge does not exist, -1 if either node does not exist
int Graph::get_edge(uint64_t a, uint64_t b){
	if (a==b || nodes.find(a) == nodes.end() || nodes.find(b) == nodes.end())
		return -1;
	if (nodes[a].find(b) == nodes[a].end())
		return 0;
	return 1;
}

// return 1 on success, -1 if the node does not exist
int Graph::get_neighbors(uint64_t node_id, vector<uint64_t>& res){
	if (nodes.find(node_id) == nodes.end())
		return -1;
	res.assign(nodes[node_id].begin(), nodes[node_id].end());
	return 1;
}

// return <distance> on success, -1 if no path, -2 if either nodes does not exist
int Graph::shortest_path(uint64_t a, uint64_t b){
	if (nodes.find(a) == nodes.end() || nodes.find(b) == nodes.end())
		return -2;
	vector<uint64_t> q;
	int head;
	map<uint64_t, int> dis;
	q.push_back(a);
	dis[a] = 0;
	for (int head = 0; head < q.size(); head++){
		uint64_t x = q[head];
		for (set<uint64_t>::iterator it = nodes[x].begin(); it != nodes[x].end(); it++){
			uint64_t y = *it;
			if (dis.find(y) == dis.end()){
				q.push_back(y);
				dis[y] = dis[x] + 1;
				if (y == b)
					return dis[y];
			}
		}
	}
	return -1;
}