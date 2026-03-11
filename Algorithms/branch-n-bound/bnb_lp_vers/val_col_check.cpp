//val_col_check.cpp

#include "val_col_check.h"

bool valid_coloring_check(Graph & g, unsigned int k, Coloring & c){
	Graph kdist = kgraph(k,g);
	int n = kdist.num_nodes();
	for(int i = 0; i < n; i++){
		for(auto nbr : kdist.get_node(i).adjacent_nodes()){
			int nbr_id = nbr.id();
			int dist = (int)nbr.edge_weight();
			if(i > nbr_id){//to avoid checking for the same pair of vertices twice
				int color1 = c.get(i);
				int color2 = c.get(nbr_id);
				if(abs(color1 - color2) <= (k - dist)){
					std::cout << "vertices: "  << i << "-" << nbr_id << " colors: " << color1 << " and " << color2 << " distance: " << dist << " and k is: "<< k <<"\n";
					return false;
				}
			}
		}
	}
	return true;
}
