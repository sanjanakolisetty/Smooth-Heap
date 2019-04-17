#ifndef graph_hpp
#define graph_hpp

#include <stdio.h>
#include <vector>

using namespace std;

struct GraphNode {
    int index;
    int x_coord;
    int y_coord;
    int degree = 0;
    bool visited = false;
    vector<pair<double, int>> adj;
};


struct Graph {
    vector<GraphNode> vertices;
};

#endif /* graph_hpp */
