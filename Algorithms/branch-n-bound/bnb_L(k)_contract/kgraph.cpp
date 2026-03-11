//kgraph.cpp
//calculates G^k for a given undirected connected graph G

#include "kgraph.h"

Graph kgraph(unsigned int k, Graph & G){
	//first step - calculating distances between nodes
	int num_vert = G.num_nodes();
	std::vector<int> allmax(num_vert,num_vert);
	std::vector<std::vector<int>> distances(num_vert,allmax);//all distances are set to maximamum at initiation
	for(int i = 0; i < num_vert; i++){//every vertex is at distances 0 to itself
		distances[i][i] = 0;
	}
	std::vector<bool> all_false(num_vert,false);
	for(int i = 0; i < num_vert; i++){//rub BFS for each vertex
		std::queue<Graph::NodeId> q;
		std::vector<bool> visited = all_false;//visited vertices are stored here
		visited[i] = true;//first vertex visited
		q.push(i);
		while(!q.empty()){
			auto cur_nodeid = q.front();
			q.pop();
			for (auto neighbor: G.get_node(cur_nodeid).adjacent_nodes()){
				if (not visited[neighbor.id()]) {
					visited[neighbor.id()] = true;
					distances[i][neighbor.id()] = distances[i][cur_nodeid] + 1;
					q.push(neighbor.id());
				}
			}
		}
	}
	//second step, create the k-th power graph
	Graph out(num_vert,Graph::undirected);
	for(int i = 0; i < num_vert; i++){
		for(int j = 0; j < num_vert; j++){
			if(distances[i][j] <= k and j > i){
				out.add_edge(i,j,(double)distances[i][j]);
			}
		}
	}
	return out;
}
