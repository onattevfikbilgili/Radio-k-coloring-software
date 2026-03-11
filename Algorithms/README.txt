This directory includes the source code and binaries for graph radio-k coloring problem algorithms mentioned in "L(2,1)-Labeling Problem with and without color lists", as well as the used test instances.

All of the included code is written for the linux-x86_64 platform.
To compile, go the directory in which the source code is included, and run the build.sh script.
Alternatively, precompiled binaries are included in the "binaries" file.

To run a program on a test graph run
$./BINARYNAME INSTANCENAME k
in your shell, where BINARYNAME is the name of the binary that you want to run,
INSTANCENAME is the file where the graph is saved
and k is a natural number, for which a graph radio-k coloring is to be computed
The input graph should be connected for the algorithm to function correctly.

Following are the abbreviations used for the algorithm directory names:
assign - assignment-based ILP formulation for the graph radio-k coloring problem
pop-k - partial-ordering-based ILP formulation for the graph radio-k coloring problem
mixed-k - mixed ILP formulation for the graph radio-k coloring problem
bnb_no_lb - branch and bound algorithm with no lower bounds used
bnb_lp_vers - branch and bound algorithm with LPs used for lower bounds
bnb_L(k)_contract - branch and bound algorithm where $\omega(G) \cdot k$ is used a lower bound
bnb_G^k_contract - branch and bound algorithm where $\omega(G^k)$ is used a lower bound

