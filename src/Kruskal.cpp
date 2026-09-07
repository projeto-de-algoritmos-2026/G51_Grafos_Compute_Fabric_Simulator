#include "../include/Kruskal.hpp"
#include <algorithm>

MSTResult Kruskal::computeMST(int numVertices, std::vector<Edge>& edges) {
    MSTResult result;
    result.totalCost = 0;

    // Ordena as arestas pelo menor peso
    std::sort(edges.begin(), edges.end());

    UnionFind dsu(numVertices);

    for (const auto& edge : edges) {
        if (dsu.unite(edge.u, edge.v)) {
            result.mstEdges.push_back(edge);
            result.totalCost += edge.weight;
            if (static_cast<int>(result.mstEdges.size()) == numVertices - 1) {
                break;
            }
        }
    }

    return result;
}