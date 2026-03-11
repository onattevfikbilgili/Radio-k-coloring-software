# Radio-k-coloring-software
Graph radio-k vertex coloring algorithms that I have implemented for my Bachelor's thesis at university of Bonn.

Every algorithm contains a bash script to compile the software.

The implementation of graph structures are taken from https://www.or.uni-bonn.de/~hougardy/alma/alma_eng.html based on the book "Algorithmic Mathematics" by Jens Vygen and Stefan Hougardy.



Hougardy, S. and Vygen, J.,Algorithmic Mathematics, (2016) 80-83

Some of these algorithms make use of CPLEX to solve linear /intiger-linear programs. To compile those add the necessary CPLEX files to include and lib directories respectively.

default usage for the algorithms:

./binary_name instance_name k

naming explanation:
assign - Assignment based ILP formulation
pop-k - Partial ordering based ILP formulation
mixed-k - Hybrid between both models
bnb-no-lb - Branch and bound algorithm with no lower bounds
bnb_L(k)_contract - Branch and bound algorithm with a heuristic clique number in G * k as a lower bound
bnb_G^k_contract - Branch and bound algorithm with a heuristic clique number in power graph G ^ k as a lower bound
bnb_lp_vers - Branch and bound algorithm with LP relaxation of assign ILP formulation as a lower bound.
