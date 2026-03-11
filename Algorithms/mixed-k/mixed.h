//mixed.h
//implementation of mixed partial ordering and assignment based ILP model for the graph radio-k coloring problem

#include <ilcplex/ilocplex.h>
#include <ilcplex/cplex.h>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <cmath>
#include "graph.h"
#include "kgraph.h"
#include "coloring.h"
#include "sp1.h"

#pragma once

int smaller(int x, int y);

Coloring mixed_ilp(unsigned int k, Graph G);
