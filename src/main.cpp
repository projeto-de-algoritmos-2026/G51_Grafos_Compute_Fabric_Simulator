#include "../include/Simulator.hpp"
#include "../include/VisualizerSVG.hpp"
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <cmath>

// Helper to generate synthetic racks in a 2D grid for Stage 1
std::vector<Rack> generateSyntheticRacks(int count) {
    std::vector<Rack> racks;
    racks.reserve(count);
    int gridSize = static_cast<int>(std::ceil(std::sqrt(count)));
    
    for (int i = 0; i < count; ++i) {
        int x = (i % gridSize) * 10;
        int y = (i / gridSize) * 10;
        racks.push_back({i, x, y});
    }
    return racks;
}

// Helper to generate a connected random network graph for Stage 2
Graph generateSyntheticNetwork(int numNodes, int edgesPerNode, unsigned int seed) {
    Graph graph(numNodes);
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> weightDist(1, 50);
    std::uniform_int_distribution<int> nodeDist(0, numNodes - 1);

    // Ensure connectivity via a basic backbone chain
    for (int i = 0; i < numNodes - 1; ++i) {
        graph.addEdge(i, i + 1, weightDist(rng));
    }

    // Add random cross-edges to simulate dense fabric interconnects
    for (int u = 0; u < numNodes; ++u) {
        for (int k = 0; k < edgesPerNode; ++k) {
            int v = nodeDist(rng);
            if (u != v) {
                graph.addEdge(u, v, weightDist(rng));
            }
        }
    }
    return graph;
}

int main() {
    Simulator sim;
    int choice = 0;

    std::cout << "========================================\n";
    std::cout << "   COMPUTE FABRIC SIMULATOR & TESTER    \n";
    std::cout << "========================================\n";
    std::cout << "1. Executar 10 Snapshots DETERMINISTICOS (Seed Fixa)\n";
    std::cout << "2. Executar 10 Snapshots NAO-DETERMINISTICOS (Cenarios Aleatorios)\n";
    std::cout << "3. Executar Benchmark de Carga Massivo (100 a 3000 nos)\n";
    std::cout << "4. Executar com dados estaticos de data/\n";
    std::cout << "Escolha uma opcao (1, 2, 3 ou 4): ";
    std::cin >> choice;

    Graph netGraph(0);
    int masterNode = 0;

    if (choice == 1 || choice == 2) {
        bool racksLoaded = sim.loadRacksFromFile("data/input_racks.txt");
        bool networkLoaded = sim.loadNetworkFromFile("data/input_network.txt", netGraph, masterNode);

        if (racksLoaded && networkLoaded) {
            bool isDeterministic = (choice == 1);
            sim.runDynamicSimulation(netGraph, masterNode, 10, isDeterministic);
        } else {
            std::cerr << "[ERRO] Falha ao carregar arquivos de data/ para a simulação.\n";
        }
    } else if (choice == 3) {
        std::cout << "\n========================================================\n";
        std::cout << "   BENCHMARK DE CARRA MASSIVA (100 A 3000 NOS)          \n";
        std::cout << "========================================================\n";

        std::vector<int> testSizes = {100, 250, 500, 1000, 2000, 3000};

        std::cout << std::left 
                  << std::setw(12) << "Nó (V)" 
                  << std::setw(18) << "Kruskal MST (ms)" 
                  << std::setw(18) << "Dijkstra SPT (ms)" 
                  << std::setw(18) << "Max Latency" << "\n";
        std::cout << std::string(66, '-') << "\n";

        for (int numNodes : testSizes) {
            // 1. Stage 1 Benchmark: Kruskal MST on Euclidean Manhattan Grid
            auto syntheticRacks = generateSyntheticRacks(numNodes);
            std::vector<Edge> candidateEdges;
            for (int i = 0; i < numNodes; ++i) {
                for (int j = i + 1; j < numNodes; ++j) {
                    int dist = std::abs(syntheticRacks[i].x - syntheticRacks[j].x) + 
                               std::abs(syntheticRacks[i].y - syntheticRacks[j].y);
                    candidateEdges.push_back({syntheticRacks[i].id, syntheticRacks[j].id, dist});
                }
            }

            auto startKruskal = std::chrono::high_resolution_clock::now();
            MSTResult mstRes = Kruskal::computeMST(numNodes, candidateEdges);
            auto endKruskal = std::chrono::high_resolution_clock::now();
            double kruskalMs = std::chrono::duration<double, std::milli>(endKruskal - startKruskal).count();

            // 2. Stage 2 Benchmark: Dijkstra SPT on Dense Network Graph
            Graph benchGraph = generateSyntheticNetwork(numNodes, 4, 1337);

            auto startDijkstra = std::chrono::high_resolution_clock::now();
            RoutingResult dijkstraRes = Dijkstra::computeShortestPaths(benchGraph, 0);
            auto endDijkstra = std::chrono::high_resolution_clock::now();
            double dijkstraMs = std::chrono::duration<double, std::milli>(endDijkstra - startDijkstra).count();

            std::cout << std::left 
                      << std::setw(12) << numNodes 
                      << std::setw(18) << std::fixed << std::setprecision(3) << kruskalMs 
                      << std::setw(18) << std::fixed << std::setprecision(3) << dijkstraMs 
                      << std::setw(18) << (std::to_string(dijkstraRes.maxLatency) + " ms") << "\n";
        }
    } else if (choice == 4) {
        if (sim.loadRacksFromFile("data/input_racks.txt")) {
            sim.runStage1_Topology();
        }
        if (sim.loadNetworkFromFile("data/input_network.txt", netGraph, masterNode)) {
            sim.runStage2_Routing(netGraph, masterNode);
        }
    } else {
        std::cerr << "[ERRO] Opcao invalida. Encerrando o programa.\n";
    }

    return 0;
}