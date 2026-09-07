#include "../include/Simulator.hpp"
#include <iostream>
#include <fstream>

bool Simulator::loadRacksFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    int n;
    if (!(file >> n)) return false;

    racks.clear();
    for (int i = 0; i < n; ++i) {
        int x, y;
        file >> x >> y;
        racks.push_back({i, x, y});
    }

    return true;
}

bool Simulator::loadNetworkFromFile(const std::string& filename, Graph& outGraph, int& outSource) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;

    int v, e;
    if (!(file >> v >> e >> outSource)) return false;

    Graph g(v);
    for (int i = 0; i < e; ++i) {
        int u, dest, weight;
        file >> u >> dest >> weight;
        g.addEdge(u, dest, weight);
    }

    outGraph = g;
    return true;
}

void Simulator::runStage1_Topology() {
    std::cout << "\n=== STAGE 1: Topology & Physical Wiring (Kruskal) ===\n";
    if (racks.empty()) {
        std::cout << "[ERRO] Nenhum rack carregado.\n";
        return;
    }

    int n = static_cast<int>(racks.size());
    std::vector<Edge> allPossibleEdges;

    // Constrói o grafo completo com as distâncias de Manhattan entre todos os racks
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            int cost = racks[i].distanceTo(racks[j]);
            allPossibleEdges.push_back({i, j, cost});
        }
    }

    MSTResult mst = Kruskal::computeMST(n, allPossibleEdges);

    std::cout << "Racks Conectados: " << n << "\n";
    std::cout << "Conexões Mínimas de Fibra Ótica:\n";
    for (const auto& edge : mst.mstEdges) {
        std::cout << "  Rack " << edge.u << " <---> Rack " << edge.v 
                  << " | Comprimento do Cabo: " << edge.weight << "m\n";
    }
    std::cout << "Custo/Comprimento Total de Cabos: " << mst.totalCost << "m\n";
}

void Simulator::runStage2_Routing(const Graph& graph, int sourceNode) {
    std::cout << "\n=== STAGE 2: Network Latency & Routing (Dijkstra) ===\n";
    std::cout << "Servidor Master (Origem do Broadcast): " << sourceNode << "\n";

    RoutingResult result = Dijkstra::computeShortestPaths(graph, sourceNode);

    std::cout << "Latencias minimas alcançadas a partir do nó master:\n";
    for (size_t i = 0; i < result.distances.size(); ++i) {
        std::cout << "  Para o Servidor " << i << ": ";
        if (result.distances[i] == INF) {
            std::cout << "Inalcançável\n";
        } else {
            std::cout << result.distances[i] << " ms\n";
        }
    }
    std::cout << "Tempo total para cobrir todo o cluster (Max Delay): " 
              << result.maxLatency << " ms\n";
}