//bnb.cpp

#include "bnb.h"
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//Implementation of the Tracker Class
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Tracker::Tracker(int n){
	std::vector<int> empty = {};
	std::vector<std::vector<int>> listoflists(n,empty);
	lists = listoflists;
}

bool Tracker::is_color(int color){
	return not lists[color].empty();
}

int Tracker::representative(int color){
	if(not lists[color].empty()){
		return lists[color][0];
	}
	else{
		throw std::runtime_error("no vertices of input color\n");
	}
}

void Tracker::color(int vertex, int color){
	if(0 <= color and color < lists.size()){
		lists[color].push_back(vertex);
	}
	else{
		throw std::runtime_error("input color out of target range\n");
	}
}

void Tracker::uncolor(int color){
	if(not lists[color].empty()){
		return lists[color].pop_back();
	}
	else{
		throw std::runtime_error("no vertices of input color\n");
	}
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//Functions for help
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
int smaller(int x, int y){
	if(x < y){return x;}
	else{return y;}
}

int calc_u(Coloring c, unsigned int k){
	int n = c.size();
	int out = 0;
	for(int i = 0; i < n; i++){
		if(c.get(i) > out){
			out = c.get(i);
		}
	}
	return out + k;
}
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


void bnb_recursion(unsigned int k, Graph & kdist, Coloring & cur, Coloring & opt, int & upper_bound, std::vector<std::vector<bool>> & available_colors, Graph contract, Tracker & track, int & total_branches){
	total_branches++;
	//std::cout << "printing current partial radio-" << k <<" coloring:\n";
	//cur.print();
	int n = kdist.num_nodes();
	bool all_vertices_are_colored = true;
	//std::cout << "Testing if all vertices are colored\n";
	for(int i = 0; i < n; i++){
		if(cur.get(i) == -1){
			all_vertices_are_colored = false;
			break;
		}
	}
	if(all_vertices_are_colored){
		int up = cur.max();
		//std::cout << "All vertices are colored, testing if new coloring is better\n";
		if(up < upper_bound){
			//std::cout << "New coloring is better, updating current optimum\nNew upper bound is " << up << "\n";
			upper_bound = up;
			opt = cur;
		}
	}
	else{
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		//compute a lower bound on the cost of extending the current partial radio-k coloring
		//std::cout << "Not all vertices are colored, computing a lower bound on the cost of extending the current partial radio-k coloring\n";
		int clique_number_estimate = clique_num(contract);
		int lower_bound = (clique_number_estimate - 1) * k;
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		if(lower_bound < upper_bound){
			//std::cout << "Lower bound is smaller than the upper bound, continuing on this branch\n";
			//picking a non-colored vertex to color
			int index_of_current_node = n;
			int num_available_colors_for_current_node = upper_bound + 1;
			//std::cout << "Picking a vertex to color\n";
			int u = smaller(upper_bound -1, calc_u(cur,k));
			for(int i = 0; i < n; i++){
				if(cur.get(i) == -1){
					int num_aval_colors_for_i = 0;
					for(int j = 0; j <= u; j++){
						if(available_colors[i][j]){
							num_aval_colors_for_i++;
						}
					}
					if(num_aval_colors_for_i < num_available_colors_for_current_node){
						index_of_current_node = i;
						num_available_colors_for_current_node = num_aval_colors_for_i;
					}
				}
			}
			//std::cout << "Vertex " << index_of_current_node << " has been picked to be colored\n";
			
			//coloring the vertex
			for(int color = 0; color < upper_bound; color++){
				if(available_colors[index_of_current_node][color]){
					//std::cout << "Vertex " << index_of_current_node << " has been colored " << color << "\n";
					cur.set((unsigned int)index_of_current_node,color);
					track.color(index_of_current_node,color);
					std::vector<std::vector<int>> updates;
					//update available colors
					//std::cout << "Forbidding colors based on distance conditions\n";
					for(auto nbr: kdist.get_node(index_of_current_node).adjacent_nodes()){
						int nbr_index = nbr.id();
						int distance = nbr.edge_weight();
						if(cur.get(nbr_index) == -1){
							for(int col2 = bigger(0,color - (k - distance)); col2 <= smaller(upper_bound -1, color + (k - distance));col2++){
								if(available_colors[nbr_index][col2]){
									//std::cout << "Forbidding color " << col2 << " for the vertex " << nbr_index << "\n";
									available_colors[nbr_index][col2] = false;
									std::vector<int> vec = {nbr_index, col2};
									updates.push_back(vec);
								}
							}
						}
					}
					Graph new_contract = contract_vertices(contract, index_of_current_node, track.representative(color));
					bnb_recursion(k,kdist, cur, opt, upper_bound, available_colors, new_contract, track, total_branches);
					track.uncolor(color);
					
					//std::cout << "Uncoloring vertex " << index_of_current_node << "\n";
					cur.set((unsigned int)index_of_current_node,-1);
					//re-update available colors
					
					while(not updates.empty()){
						int index = updates.size() - 1;
						int vertex_id = updates[index][0];
						int color_id = updates[index][1];
						available_colors[vertex_id][color_id] = true;
						updates.pop_back();
					}
				}
			}
		}
	}
}

Coloring bnb(unsigned int k, Graph & g, int & total_branches){
	total_branches = 0;
	int n = g.num_nodes();
	Graph contract = g;
	Graph kdist = kgraph(k,g);
	Coloring opt = sp1(k,g,kdist);
	int upper_bound = opt.max();
	Tracker track(upper_bound);
	std::vector<int> values(n,-1);//use -1 in the coloring to describe the color of uncolored vertices
	Coloring cur(values);
	std::vector<bool> init(upper_bound,true);
	//std::cout << "Done with initiation\n";
	std::vector<std::vector<bool>> available_colors(n,init);
	bnb_recursion(k,kdist, cur, opt, upper_bound, available_colors,g,track, total_branches);
	return opt;
}
