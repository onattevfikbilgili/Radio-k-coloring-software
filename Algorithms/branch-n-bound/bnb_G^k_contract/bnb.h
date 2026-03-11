//bnb.h - Branch and Bound based algorithm for the graph radio-k coloring problem - with using clique number on G^k as a lower bound
#include <vector>
#include <iostream>
#include <vector>
#include <stdexcept>
#include "graph.h"
#include "kgraph.h"
#include "coloring.h"
#include "sp1.h"
#include "contract_vertices.h"
#include "clique.h"

#pragma once
//class to keep track of colored vertices
class Tracker{
	public:
		bool is_color(int color);//tells if there is a vertex of a given color
		int representative(int color);//gives a vertex of the input color
		void color(int vertex, int color);//adds a vertex to the list of the input color
		void uncolor(int color);//removes the last vertex from the list of color
		Tracker(int n);//constructor
	private:
		std::vector<std::vector<int>> lists;//a list of list. in each list vertices that have been colored with color corresponding to lists index within higher list is stored
};

int smaller(int x, int y);

int calc_u(Coloring c, unsigned int k);

void bnb_recursion(unsigned int k, Graph & kdist, Coloring & cur, Coloring & opt, int & upper_bound, std::vector<std::vector<bool>> & available_colors, Graph contract, Tracker & track, int & total_branches);

Coloring bnb(unsigned int k, Graph & g, int & total_branches);
