#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include "Graph.hpp"
#include <vector>
#include <limits>

const int INF = std::numeric_limits<int>::max();

struct RoutingResult {
    std::vector<int> distances;
    int maxLatency;
};

class Dijkstra {
public:
    // Calcula as menores latências a partir de um nó de origem
    static RoutingResult computeShortestPaths(const Graph& graph, int source);
};

#endif // DIJKSTRA_HPP