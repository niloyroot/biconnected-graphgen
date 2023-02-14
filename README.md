# biconnected-graphgen
Bi-connected Graph Generator

The aim of this project is to help problem-setters and computer scientists generate biconnected graphs.

## Usage

### Input:
Enter three spaced values: N, M and tree_type. 
N is the number of nodes of the graph, M is the number of edges of the graph, and tree_type is a string that contains the type of the tree that the graph is built on. 

tree_type should be one of: "random", "silkworm", "binary", "tall", "star", "completeBinary", "maxDegree", "chain".

M should preferrably be >= 2*N, otherwise the graph will not be guaranteed to be biconnected.

### Output:
The first line contains 2 spaced integers N and M: the number of nodes and the number of edges respectively.
The next M lines contains 3 spaced integers u, v and w: u and v define an edge and w defines the weight of the edge.

## Some thanks
Thanks to ouuan for making their Tree Generator publicly available (https://github.com/ouuan/Tree-Generator/).
Thanks to Mike Miryanazov for making testlib.h publicly available (https://github.com/MikeMirzayanov/testlib/).
