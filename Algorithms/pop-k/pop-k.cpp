//pop-k.cpp

#include "pop-k.h"

int smaller(int x, int y){
	if(x < y){return x;}
	else{return y;}
}

Coloring pop_ilp(unsigned int k, Graph & G){
	//Define Variables
	int n = G.num_nodes();
	Graph kdist = kgraph(k,G);//G^k, where the edge weights correspond to distances of vertices in the original graph
	Coloring initial = sp1(k,G,kdist);//initial coloring using heuristic from Saha and Panigrahi
	int upper_bound = initial.max();//an upper bound on the radio-k chromatic number
	IloEnv myenv;
	IloModel mymodel(myenv);
	IloNumVar target(myenv, 0, IloInfinity, ILOINT);//variable in the ILP representing the chromatic number
	IloNumVarArray y(myenv, n * (upper_bound + 1), 0, 1, ILOINT);//variables y_{i,v} representing whether the vertex v has color strictly greater than i, the variable y[v*(upper_bound + 1) + i] represents y_{i,v}
	IloNumVarArray z(myenv, n * (upper_bound + 1), 0, 1, ILOINT);//variables z_{v,i} representing whether the vertex v has color strictly smaller than i, the variable z[v*(upper_bound + 1) + i] represents z_{v,i}
	
	//Program every vertex to have exactly one color
	for(int i = 0; i < n; i++){
		z[i * (upper_bound + 1)].setBounds(0,0);//y_{ub,v} = 0: \forall v \in V(G)
		y[i * (upper_bound + 1) + upper_bound].setBounds(0,0);//z_{v,0} = 0: \forall v \in V(G)
		for(int j = 0; j < upper_bound; j++){
			mymodel.add(IloRange(myenv,0,y[i * (upper_bound + 1) + j] - y[i * (upper_bound + 1) + j + 1],IloInfinity));//y_{i,v} - y_{i+1,v} \geq 0: \forall i \in V(G), \forall i \in {0,...,ub -1\}
			mymodel.add(IloRange(myenv,1,y[i * (upper_bound + 1) + j] + z[i * (upper_bound + 1) + j +1],1));//y_{i,v} + z_{v,i+1} = 1: \forall v \in V(G), \forall i \in {0,...,ub -1\}
		}
	}
	
	//Program the distance conditions
	for(int v = 0; v < n; v++){
		for(auto nbr: kdist.get_node(v).adjacent_nodes()){
			int w = nbr.id();
			int dist = (int)nbr.edge_weight();
			if(v > w){
				for(int color1 = 0; color1 <= upper_bound; color1++){
					for(int color2 = bigger(0,color1 - (k - dist)); color2 <= smaller(upper_bound, color1 + (k - dist)); color2++){//finds incompatible pairs of colors on the vertices at distance less than k
						mymodel.add(IloRange(myenv,1,y[v * (upper_bound + 1) + color1] + z[v * (upper_bound + 1) + color1] + y[w * (upper_bound + 1) + color2] + z[w * (upper_bound + 1) + color2],IloInfinity));
					}
				}
			}
		}
	}
	
	//Program the constraints on the variable representing the radio-k chromatic number
	for(int v = 0; v < n; v++){
		for(int i = 0; i <= upper_bound; i++){
			mymodel.add(IloRange(myenv,-IloInfinity,i - i * y[v * (upper_bound + 1) + i] - i * z[v * (upper_bound + 1) + i] - target,0));
		}
	}
	
	//Program the objective function
	mymodel.add(IloMinimize(myenv, target));
	
	//Solving the ILP model
	IloCplex mycplex(myenv);
	mycplex.extract(mymodel);
	mycplex.setOut(myenv.getNullStream());
	IloBool feasible = mycplex.solve();
	if(feasible == IloFalse){throw std::runtime_error("ILP not feasible!");}
	
	//Compute the coloring corresponding to the solution of our ILP
	std::vector<int> myvec(n,0);
	Coloring out(myvec);
	for(int i = 0; i < n; i++){
		for(int j = 0; j <= upper_bound; j++){
			if(round(mycplex.getValue(y[i *(upper_bound + 1) + j])) == 0 and round(mycplex.getValue(z[i *(upper_bound + 1) + j])) == 0){
				out.set(i,j);
			}
		}
	}
	mycplex.clear();
	myenv.end();

	//return the result
	return out;
}
