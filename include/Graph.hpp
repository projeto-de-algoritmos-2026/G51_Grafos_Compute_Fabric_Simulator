#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <cmath>

// Estrutura para representar um Rack no piso do Data Center (Stage 1)
struct Rack {
    int id;
    int x;
    int y;

    // Distância de Manhattan entre dois racks
    int distanceTo(const Rack& other) const {
        return std::abs(x - other.x) + std::abs(y - other.y);
    }
};

// Estrutura de Aresta genérica
struct Edge {
    int u;
    int v;
    int weight;

    // Operador para ordenação no Kruskal
    bool operator<(const Edge& other) const {
        return weight < other.weight;
    }
};

// Representação de um nó adjacente para lista de adjacência (Dijkstra)
struct AdjNode {
    int to;
    int weight;
};

// Classe principal para gerenciar os grafos do simulador
class Graph {
private:
    int numVertices;
    std::vector<Edge> edgeList;                          // Usado para Kruskal
    std::vector<std::vector<AdjNode>> adjList;          // Usado para Dijkstra

public:
    explicit Graph(int vertices);

    void addEdge(int u, int v, int weight);
    
    int getNumVertices() const;
    const std::vector<Edge>& getEdgeList() const;
    const std::vector<std::vector<AdjNode>>& getAdjList() const;
};

#endif // GRAPH_HPP