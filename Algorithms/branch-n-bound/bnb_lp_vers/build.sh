#!/bin/sh
#Version 2211 of CPLEX was used for this project
#all necessary files for cplex are included within the directories include and lib
#to run the programm type $./branch_bound_lp FILENAME k
#in your shell where FILENAME is the name of the file where the graph is stored
g++ -I./include -o branch_bound_lp *.cpp -L./lib -lilocplex -lconcert -lcplex -lm -lpthread
