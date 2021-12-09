#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include "Artists.h"
using namespace std;

struct Edge {
    int to, from;
    Edge(int _to, int _from) {to = _to; from = _from;}
};

struct Graph {
    int numVertices;
    vector<vector<int>> adjList;

    Graph(const vector<Artists>& Artists, int vertices);
    vector<int> bfs(const Graph& graph, int src);
    vector<int> dfs(const Graph& graph, int src);
    vector<int> getAdjacent(int vertex);
};