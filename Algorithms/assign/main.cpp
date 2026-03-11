//the main file for the radio-2 coloring algorithm using an assignment based ILP formulation
#include <iostream>
#include <cstdlib>
#include <chrono>
#include "graph.h"
#include "kgraph.h"
#include "coloring.h"
#include "sp1.h"
#include "assignment.h"
#include "val_col_check.h"

int main(int argc, char** argv){
	if(argc > 1){
		Graph g(argv[1],Graph::undirected);
		unsigned int k = std::atoi(argv[2]);
		std::chrono::time_point<std::chrono::system_clock> start, end;
		start = std::chrono::system_clock::now();
		Coloring exact = assignment_ilp(k,g);
		end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end - start;
		if(valid_coloring_check(g,k,exact)){
			std::cout << "Printing a radio-" << k << " coloring of the original graph:\n";
			exact.print();
			std::cout << "Largest color used is " << exact.max() << "\n";
			std::cout << "Took " << elapsed_seconds.count() << " seconds to compute the coloring.\n";
		}
	}
	return 0;
}
