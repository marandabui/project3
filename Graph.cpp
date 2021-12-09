#include "Graph.h"
#include <vector>
#include <queue>
#include <SFML/Graphics.hpp>
using namespace std;

Graph::Graph(const vector<Artists> &artistList, int vertices) {
    numVertices = vertices;
    adjList.resize(vertices);
    for (int i = 0; i < artistList.size(); i++){
        Artists currV = artistList[i];
        for (int j = i; j < artistList.size(); j++){
            if (currV.genre == artistList[j].genre && currV.name != artistList[j].name && currV.popularity > artistList[j].popularity)
                adjList[i].push_back(j);
        }
    }
}

vector<int> Graph::bfs(const Graph& graph, int src) {
    //pseudocode from slides
    int v = graph.numVertices;
    bool *visited = new bool[v];

    for (int i = 0; i < v; i++){
        visited[i] = false;
    }

    queue<int> q;
    vector<int> vector, ret;

    visited[src] = true;
    q.push(src);

    while(!q.empty()){
        int newV = q.front();
        if (newV != src)
            ret.push_back(newV);
        q.pop();

        if (ret.size() == 10)
            return ret;

        for(int adj: graph.adjList[newV]){
            if(!visited[adj]){
                visited[adj] = true;
                vector.push_back(adj);
            }
            sort(vector.begin(),vector.end());
            for(int i=0;i<vector.size();i++){
                q.push(vector.back());
            }
            vector.clear();
        }
    }
    return ret;
}

vector<int> Graph::dfs(const Graph& graph, int src) {
    // from aman's slides
    int v = graph.numVertices;
    bool *visited = new bool[v];

    for (int i = 0; i < v; i++){
        visited[i] = false;
    }

    std::stack<int> s;
    std::vector<int> vector, ret;

    visited[src] = true;
    s.push(src);

    while(!s.empty()){
        int newV = s.top();
        if (newV != src)
            ret.push_back(newV);
        s.pop();

        if (ret.size() == 10)
            return ret;

        for(int adj: graph.adjList[newV]){
            if(!visited[adj]){
                visited[adj] = true;
                vector.push_back(adj);
            }
            sort(vector.begin(),vector.end());
            for(int i=0;i<vector.size();i++){
                s.push(vector.back());
            }
            vector.clear();
        }
    }
    return ret;
}

vector<int> Graph::getAdjacent(int vertex) {
    vector<int> result;
    vector<int> temp = adjList[vertex];
    for (int i = 0; i < adjList[vertex].size(); i++){
        result.push_back(temp[i]);
    }
    sort(result.begin(), result.end());
    return result;
}