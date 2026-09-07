#include "../include/Dijkstra.hpp"
#include <queue>
#include <algorithm>

RoutingResult Dijkstra::computeShortestPaths(const Graph& graph, int source) {
    int n = graph.getNumVertices();
    std::vector<int> dist(n, INF);
    dist[source] = 0;

    // Min-priority queue armazena pares: (distância, nó)
    using PII = std::pair<int, int>;
    std::priority_queue<PII, std::vector<PII>, std::greater<PII>> pq;

    pq.push({0, source});

    const auto& adj = graph.getAdjList();

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;

        for (const auto& neighbor : adj[u]) {
            int v = neighbor.to;
            int weight = neighbor.weight;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }

    int maxLat = 0;
    for (int d : dist) {
        if (d != INF) {
            maxLat = std::max(maxLat, d);
        }
    }

    return {dist, maxLat};
}