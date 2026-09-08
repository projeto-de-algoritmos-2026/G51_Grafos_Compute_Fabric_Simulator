#include "../include/Graph.hpp"

Graph::Graph(int vertices) : numVertices(vertices) {
    adjList.resize(vertices);
}

void Graph::addEdge(int u, int v, int weight) {
    if (u >= 0 && u < numVertices && v >= 0 && v < numVertices) {
        adjList[u].push_back({v, weight});
        adjList[v].push_back({u, weight});
        edgeList.push_back({u, v, weight});
    }
}

void Graph::updateEdgeWeight(int u, int v, int newWeight) {
    if (u < 0 || u >= numVertices || v < 0 || v >= numVertices) return;

    // Atualiza u -> v
    for (auto& neighbor : adjList[u]) {
        if (neighbor.to == v) {
            neighbor.weight = newWeight;
            break;
        }
    }

    // Atualiza v -> u
    for (auto& neighbor : adjList[v]) {
        if (neighbor.to == u) {
            neighbor.weight = newWeight;
            break;
        }
    }

    // Atualiza a lista de arestas planas (Kruskal)
    for (auto& edge : edgeList) {
        if ((edge.u == u && edge.v == v) || (edge.u == v && edge.v == u)) {
            edge.weight = newWeight;
            break;
        }
    }
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