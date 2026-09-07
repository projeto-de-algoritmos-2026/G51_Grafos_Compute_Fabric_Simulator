#include "../include/Simulator.hpp"
#include "../include/VisualizerSVG.hpp"
#include <iostream>
#include <chrono>
#include <random>
#include <fstream>

void runBenchmark(int numRacks) {
    std::cout << "\n==================================================\n";
    std::cout << " RUNNING BENCHMARK (" << numRacks << " RACKS / ~" << (numRacks * (numRacks - 1)) / 2 << " EDGES)\n";
    std::cout << "==================================================\n";

    std::vector<Rack> racks;
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> distCoord(0, 1000);

    for (int i = 0; i < numRacks; ++i) {
        racks.push_back({i, distCoord(rng), distCoord(rng)});
    }

    // --- BENCHMARK STAGE 1: KRUSKAL ---
    auto startKruskal = std::chrono::high_resolution_clock::now();

    std::vector<Edge> allEdges;
    allEdges.reserve((numRacks * (numRacks - 1)) / 2);
    for (int i = 0; i < numRacks; ++i) {
        for (int j = i + 1; j < numRacks; ++j) {
            allEdges.push_back({i, j, racks[i].distanceTo(racks[j])});
        }
    }

    MSTResult mst = Kruskal::computeMST(numRacks, allEdges);

    auto endKruskal = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationKruskal = endKruskal - startKruskal;

    std::cout << "[Stage 1 - Kruskal] Tempo de Execucao: " << durationKruskal.count() << " ms\n";
    std::cout << "  - Arestas Processadas: " << allEdges.size() << "\n";
    std::cout << "  - Custo Total da MST: " << mst.totalCost << "m\n";

    // Exporta SVG apenas para escalas visualizáveis (até 500 nós)
    if (numRacks <= 500) {
        VisualizerSVG::exportMSTToSVG("topology_output.svg", racks, mst.mstEdges);
        std::cout << "  - Visualizacao 2D salva como 'topology_output.svg'\n";
    }

    // --- BENCHMARK STAGE 2: DIJKSTRA ---
    Graph g(numRacks);
    for (const auto& e : mst.mstEdges) {
        g.addEdge(e.u, e.v, e.weight);
    }

    auto startDijkstra = std::chrono::high_resolution_clock::now();

    RoutingResult routing = Dijkstra::computeShortestPaths(g, 0);

    auto endDijkstra = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> durationDijkstra = endDijkstra - startDijkstra;

    std::cout << "[Stage 2 - Dijkstra] Tempo de Execucao: " << durationDijkstra.count() << " ms\n";
    std::cout << "  - Maior Latencia Encontrada: " << routing.maxLatency << " ms\n";
}

int main() {
    int choice = 0;

    std::cout << "========================================\n";
    std::cout << "   COMPUTE FABRIC SIMULATOR & BENCHMARK \n";
    std::cout << "========================================\n";
    std::cout << "1. Executar Benchmark com Grande Volume de Dados\n";
    std::cout << "2. Executar com arquivos de data/\n";
    std::cout << "Escolha uma opcao (1 ou 2): ";
    std::cin >> choice;

    if (choice == 1) {
        int sizes[] = {100, 500, 1000, 3000};
        for (int size : sizes) {
            runBenchmark(size);
        }
    } else {
        Simulator sim;
        if (sim.loadRacksFromFile("data/input_racks.txt")) {
            sim.runStage1_Topology();
        }
        Graph netGraph(0);
        int masterNode = 0;
        if (sim.loadNetworkFromFile("data/input_network.txt", netGraph, masterNode)) {
            sim.runStage2_Routing(netGraph, masterNode);
        }
    }

    return 0;
}