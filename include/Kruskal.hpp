#ifndef KRUSKAL_HPP
#define KRUSKAL_HPP

#include "Graph.hpp"
#include "UnionFind.hpp"
#include <vector>

struct MSTResult {
    std::vector<Edge> mstEdges;
    int totalCost;
};

class Kruskal {
public:
    // Executa o algoritmo de Kruskal para encontrar a Árvore Geradora Mínima
    static MSTResult computeMST(int numVertices, std::vector<Edge>& edges);
};

#endif // KRUSKAL_HPP