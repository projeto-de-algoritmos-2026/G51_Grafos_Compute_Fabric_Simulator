#include "../include/Dijkstra.hpp"
#include <queue>
#include <limits>
#include <algorithm>

RoutingResult Dijkstra::computeShortestPaths(const Graph& graph, int sourceNode) {
    int n = graph.getNumVertices();
    std::vector<int> dist(n, std::numeric_limits<int>::max());
    std::vector<int> parent(n, -1); // <--- INICIALIZA ANTECESSORES COM -1

    // Fila de prioridade: min-heap (distancia, vertice)
    using pii = std::pair<int, int>;
    std::priority_queue<pii, std::vector<pii>, std::greater<pii>> pq;

    dist[sourceNode] = 0;
    pq.push({0, sourceNode});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;

        for (const auto& edge : graph.getAdjList()[u]) {
            int v = edge.to;
            int weight = edge.weight;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u; // <--- REGISTRA O ANTECESSOR NO CAMINHO MINIMO
                pq.push({dist[v], v});
            }
        }
    }

    int maxLat = 0;
    for (int d : dist) {
        if (d != std::numeric_limits<int>::max()) {
            maxLat = std::max(maxLat, d);
        }
    }

    return {dist, parent, maxLat}; // <--- RETORNA PARENT NA STRUCT
}