//bnb.h - Branch and Bound based algorithm for the graph radio-k coloring problem - without using any lower bounds
#include <vector>
#include <iostream>
#include <vector>
#include <stdexcept>
#include "graph.h"
#include "kgraph.h"
#include "coloring.h"
#include "sp1.h"

#pragma once

int smaller(int x, int y);

int calc_u(Coloring c, unsigned int k);

void bnb_recursion(unsigned int k, Graph & kdist, Coloring & cur, Coloring & opt, int & upper_bound, std::vector<std::vector<bool>> & available_colors, int & total_branches);

Coloring bnb(unsigned int k, Graph & g, int & total_branches);
