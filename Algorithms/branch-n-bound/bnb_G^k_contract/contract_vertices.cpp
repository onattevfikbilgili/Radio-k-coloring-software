//contract_vertices.cpp

#include "contract_vertices.h"

Graph contract_vertices(Graph g, Graph::NodeId v_1, Graph::NodeId v_2){
	if(v_1 == v_2){
		Graph out = g;
		return out;
	}
	int n = g.num_nodes();
	Graph out(n,Graph::undirected);
	for(int i = 0; i < n; i++){
		if(i != v_2){
			for(auto nbr : g.get_node(i).adjacent_nodes()){
				int nbr_id = nbr.id();
				int nbr_wgt = nbr.edge_weight();
				if(i < nbr_id and nbr_id != v_2){//to avoid parallel edges and edges incident to v_2 
					out.add_edge(i,nbr_id,nbr_wgt);
				}
			}
		}
	}
	for(auto nbr : g.get_node(v_2).adjacent_nodes()){
		int nbr_id = nbr.id();
		int nbr_wgt = nbr.edge_weight();
		out.add_edge(v_1, nbr_id, nbr_wgt);// all edges previously incident to v_2 are now incident to v_1
	}
	return out;
}
