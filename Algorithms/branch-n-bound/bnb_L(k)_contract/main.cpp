#include <iostream>
#include <cstdlib>
#include <chrono>
#include "graph.h"
#include "bnb.h"
#include "val_col_check.h"

int main(int argc, char** argv){
	if(argc > 2){
		Graph mygraph(argv[1],Graph::undirected);
		unsigned int k = std::atoi(argv[2]);
		std::chrono::time_point<std::chrono::system_clock> start, end;
		int total_branches = 0;
		start = std::chrono::system_clock::now();
		Coloring exact = bnb(k,mygraph, total_branches);
		end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end - start;
		if(valid_coloring_check(mygraph,k,exact)){
			std::cout << "Printing an exact radio-" << k << " coloring of the original graph:\n";
			exact.print();
			std::cout << "Largest color used is " << exact.max() << "\n";
			std::cout << total_branches <<" partial colorings observed throughout the runtime of this algorithm.\n";
			std::cout << "Took " << elapsed_seconds.count() << " seconds to compute the coloring.\n";
		}
	}
}
