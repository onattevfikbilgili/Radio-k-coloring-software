//clique.h - a heuristic to estimate the size of a cardinality maximal clique

#include <vector>
#include "graph.h"

#pragma once

bool is_edge(int v, int w, Graph G);

int clique_num(Graph g); 
