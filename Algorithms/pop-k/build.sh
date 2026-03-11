#!/bin/sh
#Version 2211 of cplex studio was used for this project
#all necessary files for cplex are included within the directories include and lib
#computes a graph radio-k coloring of the input graph
#to run the programm type $./popk FILENAME k
#in your shell where FILENAME is the name of the file where the graph is stored
g++ -I./include -o popk main.cpp graph.cpp kgraph.cpp coloring.cpp sp1.cpp pop-k.cpp val_col_check.cpp -L./lib -lilocplex -lconcert -lcplex -lm -lpthread
