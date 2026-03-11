//assignment.cpp

#include "assignment.h"

int smaller(int x, int y){
	if(x < y){return x;}
	else{return y;}
}

Coloring assignment_ilp(unsigned int k, Graph & G){
	int n = G.num_nodes();
	Graph kdist = kgraph(k,G);
	Coloring initial = sp1(k,G,kdist);//initial coloring using heuristic from Saha and Panigrahi
	int upper_bound = initial.max();//an upper bound on the radio-k chromatic number
	IloEnv myenv;
	IloModel mymodel(myenv);
	IloNumVar target(myenv, 0, upper_bound, ILOINT);//variable in the ILP representing the chromatic number
	IloNumVarArray variables(myenv, n * (upper_bound + 1), 0, 1, ILOINT);//variables x_{v,i} representing whether the vertex v has color i
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
	//Programm the objective function
	mymodel.add(IloMinimize(myenv, target));

	//Solving the ILP model
	IloCplex mycplex(myenv);
	mycplex.extract(mymodel);
	mycplex.setOut(myenv.getNullStream());
	IloBool feasible = mycplex.solve();
	if(feasible == IloFalse){throw std::runtime_error("ILP not feasible!");}
	

	//Compute the coloring corresponding to the solution of our ILP
	std::vector<int> myvec(n,1234567890);
	Coloring out(myvec);
	for(int i = 0; i < n; i++){
		for(int j = 0; j <= upper_bound; j++){
			if(round(mycplex.getValue(variables[i * (upper_bound + 1) + j])) == 1){
				out.set(i,j);
			}
		}
	}

	mycplex.clear();
	myenv.end();

	//return the result
	return out;
}
