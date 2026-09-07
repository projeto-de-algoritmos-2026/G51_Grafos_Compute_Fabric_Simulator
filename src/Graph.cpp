#include "../include/Graph.hpp"

Graph::Graph(int vertices) : numVertices(vertices) {
    adjList.resize(vertices);
}

void Graph::addEdge(int u, int v, int weight) {
    edgeList.push_back({u, v, weight});
    adjList[u].push_back({v, weight});
    adjList[v].push_back({u, weight}); // Grafo não-direcionado para o Data Center
}

int Graph::getNumVertices() const {
    return numVertices;
}

const std::vector<Edge>& Graph::getEdgeList() const {
    return edgeList;
}

const std::vector<std::vector<AdjNode>>& Graph::getAdjList() const {
    return adjList;
}