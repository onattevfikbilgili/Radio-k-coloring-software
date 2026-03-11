//bnb.cpp

#include "bnb.h"

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

void bnb_recursion(unsigned int k, Graph & kdist, Coloring & cur, Coloring & opt, int & upper_bound, std::vector<std::vector<bool>> & available_colors, int & total_branches){
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
//This is done by solving an LP relaxation of the ILP formulation for this problem
//Every vertex that has been colored will have fixed coloring in this formulation
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		int lower_bound;
		IloEnv myenv;
		IloModel mymodel(myenv);
		IloNumVar target(myenv, 0, upper_bound, ILOFLOAT);//variable in the ILP representing the chromatic number
		IloNumVarArray variables(myenv, n * (upper_bound + 1), 0, 1, ILOFLOAT);//variables x_{v,i} representing whether the vertex v has color i
																			//variables[v * (upper_bound + 1) + i] represents x_{v,i}
		//Programm every vertex to have exactly one color
		for(int i = 0; i < n; i++){
			IloExpr onecolor(myenv);
			for(int j = 0; j <= upper_bound; j++){
				onecolor += variables[i * (upper_bound + 1) + j];
			}
			mymodel.add(IloRange(myenv, 1, onecolor, 1));
			onecolor.end();
		}
		
		//Programm the distance conditions
		for(int i = 0; i < n; i++){
			for(auto nbr: kdist.get_node(i).adjacent_nodes()){
				int nbr_index = nbr.id();
				int nbr_dist = nbr.edge_weight();
				if(nbr_index > i){//to ensure every edge {v,w} gets considered only once
					for(int color1 = 0; color1 <= upper_bound; color1++){
						for(int color2 = bigger(0, color1 - (k - nbr_dist)); color2 <= smaller(upper_bound, color1 + (k - nbr_dist)); color2++){
							mymodel.add(IloRange(myenv,0,variables[(i * (upper_bound + 1)) + color1] + variables[(nbr_index * (upper_bound + 1)) + color2],1));
						}
					}
				}
			}
		}
		//Programm the constraints on the variable representing the radio-k chromatic number
		for(int i = 0; i < n; i++){
			for(int j = 0; j <= upper_bound; j++){
				mymodel.add(IloRange(myenv,-IloInfinity, j * variables[i * (upper_bound + 1) + j] - target,0));
			}
		}
		//Fix colors for the vertices that have been colored
		for(int i = 0; i < n; i++){
			if(cur.get(i) != -1){
				int color_of_vertex_i = cur.get(i);
				mymodel.add(IloRange(myenv,1,variables[i * (upper_bound + 1) + color_of_vertex_i],1));
			}
		}
		//Programm the objective function
		mymodel.add(IloMinimize(myenv, target));
		//Solving the LP relaxation of the ILP model
		IloCplex mycplex(myenv);
		mycplex.extract(mymodel);
		mycplex.setOut(myenv.getNullStream());
		IloBool feasible = mycplex.solve();
		//derive the lower bound
		if(feasible == IloFalse){lower_bound = upper_bound;}
		else{lower_bound = (int)mycplex.getValue(target);}
		//End the LP
		mycplex.clear();
		myenv.end();
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
					bnb_recursion(k,kdist, cur, opt, upper_bound, available_colors,total_branches);
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
	Graph kdist = kgraph(k,g);
	Coloring opt = sp1(k,g,kdist);
	int upper_bound = opt.max();
	std::vector<int> values(n,-1);//use -1 in the coloring to describe the color of uncolored vertices
	Coloring cur(values);
	std::vector<bool> init(upper_bound,true);
	//std::cout << "Done with initiation\n";
	std::vector<std::vector<bool>> available_colors(n,init);
	bnb_recursion(k,kdist, cur, opt, upper_bound, available_colors, total_branches);
	return opt;
}
