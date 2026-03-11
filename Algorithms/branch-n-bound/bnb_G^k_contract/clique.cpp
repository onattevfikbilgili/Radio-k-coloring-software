//clique.cpp

#include "clique.h"

bool is_edge(int v, int w, Graph G){
	bool out = false;
	for(auto nbr : G.get_node(v).adjacent_nodes()){
		int index = nbr.id();
		if(index == w){
			out = true;
			break;
		}
	}
	return out;
}

int clique_num(Graph g){
	int n = g.num_nodes();
	int out = 0;
	for(int i = 0; i < n; i++){//iterate over all vertices
		std::vector<int> clique_nodes = {i};//add current vertex to the clique nodes and use a greedy algorithm to find an inclusion-wise maximal clique
		int cur_size = 1;
		for(auto nbr : g.get_node(i).adjacent_nodes()){
			int nbr_id = nbr.id();
			bool is_candidate = true;
			for(int j = 0; j < clique_nodes.size() and is_candidate; j++){
				if(not is_edge(i,nbr_id,g)){//if the candidate is not adjacent to all clique nodes, it is no longer a candidate for the clique;
					is_candidate = false;
				}
			}
			clique_nodes.push_back(nbr_id);//add candidate to the clique
		}
		if(cur_size > out){
			out = cur_size;
		}
	}
	return out;
}
