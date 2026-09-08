#include "../include/Simulator.hpp"
#include "../include/VisualizerSVG.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <random>
#include <iomanip>
#include <filesystem>
#include <cmath>
#include <random>

namespace fs = std::filesystem;

bool Simulator::loadRacksFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[ERRO] Não foi possível abrir o arquivo: " << filename << "\n";
        return false;
    }

    racks.clear();
    int totalRacks;
    if (!(file >> totalRacks)) return false;

    int id, x, y;
    while (file >> id >> x >> y) {
        racks.push_back({id, x, y});
    }

    file.close();
    return !racks.empty();
}

bool Simulator::loadNetworkFromFile(const std::string& filename, Graph& outGraph, int& outSource) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[ERRO] Não foi possível abrir o arquivo: " << filename << "\n";
        return false;
    }

    int numNodes, numEdges;
    // Lendo a ordem exata do input_network.txt: 50 0 105 (Nós, Master, Arestas)
    if (!(file >> numNodes >> outSource >> numEdges)) {
        return false;
    }

    outGraph = Graph(numNodes);

    int u, v, weight;
    int loadedEdges = 0;
    while (file >> u >> v >> weight) {
        outGraph.addEdge(u, v, weight);
        loadedEdges++;
    }

    file.close();
    return loadedEdges > 0;
}

void Simulator::runStage1_Topology() {
    std::cout << "\n========================================================\n";
    std::cout << "   STAGE 1: PHYSICAL TOPOLOGY OPTIMIZATION (KRUSKAL)    \n";
    std::cout << "========================================================\n";

    if (racks.empty()) {
        std::cerr << "[ERRO] Nenhum rack carregado para o Stage 1.\n";
        return;
    }

    int n = racks.size();
    std::vector<Edge> candidateEdges;

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            int dist = std::abs(racks[i].x - racks[j].x) + std::abs(racks[i].y - racks[j].y);
            candidateEdges.push_back({racks[i].id, racks[j].id, dist});
        }
    }

    MSTResult result = Kruskal::computeMST(n, candidateEdges);

    std::cout << "Número de Racks: " << n << "\n";
    std::cout << "Total de Conexões na MST: " << result.mstEdges.size() << "\n";
    std::cout << "Custo Total do Cabeamento: " << result.totalCost << " m\n";

    VisualizerSVG::exportTopologyToSVG("topology_output.svg", racks, result.mstEdges);
}

void Simulator::runStage2_Routing(const Graph& graph, int sourceNode) {
    std::cout << "\n========================================================\n";
    std::cout << "   STAGE 2: ROUTING & LATENCY ANALYSIS (DIJKSTRA)       \n";
    std::cout << "========================================================\n";

    RoutingResult result = Dijkstra::computeShortestPaths(graph, sourceNode);

    std::cout << "Nó Servidor de Origem (Master): " << sourceNode << "\n";
    std::cout << "Latência Máxima de Broadcast: " << result.maxLatency << " ms\n";
}

// Função auxiliar para dar aquele "sabor" orgânico às coordenadas do Grid
void temperarCoordenadas(std::vector<Rack>& racks) {
    std::mt19937 rng(std::random_device{}()); // Seed fixa para manter consistente a cada run
    
    // Pequenos deslocamentos (Jitter de +/- 15px)
    std::uniform_int_distribution<int> jitter(-15, 15);
    
    // Leve rotação de corredor (+/- 5px)
    std::uniform_int_distribution<int> aisleOffset(-5, 5);

    for (auto& r : racks) {
        // 1. Grid base de 10 colunas x 5 linhas
        int col = r.id % 10;
        int row = r.id / 10;

        // 2. Coordenadas base expandidas para resolução SVG
        int baseX = 100 + col * 80;
        int baseY = 100 + row * 140;

        // 3. O SABOR:
        // - Corredores Quentes/Frios: afasta um pouco as linhas pares das ímpares
        int hotAisle = (row % 2 == 0) ? -20 : 20;

        // - Curvatura orgânica do Data Center (onda senoidal leve na parede)
        int organicWave = static_cast<int>(std::sin(col * 0.5f) * 12.0f);

        // - Ruído aleatório por servidor (cabo folgado, rack levemente torto)
        int noiseX = jitter(rng);
        int noiseY = jitter(rng);

        // Aplica as coordenadas temperadas
        r.x = baseX + noiseX + organicWave;
        r.y = baseY + hotAisle + noiseY + aisleOffset(rng);
    }
}

void Simulator::runDynamicSimulation(
    Graph baseGraph, 
    int sourceNode, 
    int numSnapshots, 
    bool isDeterministic, 
    bool dynamicTopology,
    bool randomizeTopology) 
{
    std::string outputDir = isDeterministic ? "output_deterministic" : "output_random";
    fs::create_directories(outputDir);

    unsigned int seed = isDeterministic ? 1337 : std::random_device{}();
    std::mt19937 rng(seed);

    std::uniform_real_distribution<double> eventChance(0.0, 1.0);
    std::uniform_int_distribution<int> weightDelta(2, 12);
    std::uniform_int_distribution<int> distanceDelta(1, 5);
    std::uniform_int_distribution<int> initWeightDist(1, 35);

    Graph currentGraph = baseGraph;

    // Randomiza pesos iniciais do grafo no início da simulação se solicitado
    if (randomizeTopology) {
        int numVertices = currentGraph.getNumVertices();
        auto adj = currentGraph.getAdjList();

        for (int u = 0; u < numVertices; ++u) {
            for (const auto& neighbor : adj[u]) {
                int v = neighbor.to;
                if (u < v) {
                    currentGraph.updateEdgeWeight(u, v, initWeightDist(rng));
                }
            }
        }
    }

    // Inicializa conjunto de arestas físicas para o Kruskal
    int n = racks.size();
    std::vector<Edge> currentPhysicalEdges;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            int dist = std::abs(racks[i].x - racks[j].x) + std::abs(racks[i].y - racks[j].y);
            currentPhysicalEdges.push_back({racks[i].id, racks[j].id, dist});
        }
    }

    std::cout << "\n========================================================\n";
    std::cout << "  STAGE 1 & 2: 10 SNAPSHOTS " << (isDeterministic ? "DETERMINISTICOS" : "ALEATORIOS") << "\n";
    std::cout << "  TOPOLOGIA INICIAL: " << (randomizeTopology ? "RANDOMICA" : "ESTATICA DE DATA/") << "\n";
    std::cout << "========================================================\n";

    std::cout << std::left 
              << std::setw(10) << "Snapshot" 
              << std::setw(18) << "Custo MST (m)"
              << std::setw(20) << "Max Latency (ms)" 
              << std::setw(25) << "Status da Rede" << "\n";
    std::cout << std::string(73, '-') << "\n";

    for (int t = 1; t <= numSnapshots; ++t) {
        // Mutação contínua a partir de t > 1
        if (t > 1) {
            int numVertices = currentGraph.getNumVertices();
            auto adj = currentGraph.getAdjList();

            // Mutação in-place do grafo lógico de roteamento
            for (int u = 0; u < numVertices; ++u) {
                for (const auto& neighbor : adj[u]) {
                    int v = neighbor.to;
                    if (u < v) {
                        if (eventChance(rng) < 0.25) {
                            int newWeight = neighbor.weight + weightDelta(rng);
                            currentGraph.updateEdgeWeight(u, v, newWeight);
                        }
                    }
                }
            }

            // Degradação física dos cabos se a flag estiver ativa
            if (dynamicTopology) {
                for (auto& edge : currentPhysicalEdges) {
                    if (eventChance(rng) < 0.15) {
                        edge.weight += distanceDelta(rng);
                    }
                }
            }
        }

        RoutingResult dijkstraRes = Dijkstra::computeShortestPaths(currentGraph, sourceNode);
        MSTResult mstRes = Kruskal::computeMST(n, currentPhysicalEdges);

        std::string status = (dijkstraRes.maxLatency > 65) 
            ? "[ALERTA: Congestionamento] - ping: " + std::to_string(dijkstraRes.maxLatency) + "ms" 
            : "[OPERAÇÃO NORMAL]" + std::to_string(dijkstraRes.maxLatency) + "ms" ;

        if (!racks.empty()) {
            temperarCoordenadas(racks);
            std::string svgRouting = outputDir + "/snapshot_t" + std::to_string(t) + ".svg";
            VisualizerSVG::exportShortestPathTreeToSVG(svgRouting, racks, dijkstraRes.parent, sourceNode);

            if (dynamicTopology) {
                std::string svgTopology = outputDir + "/topology_t" + std::to_string(t) + ".svg";
                VisualizerSVG::exportTopologyToSVG(svgTopology, racks, mstRes.mstEdges);
            }
        }

        std::cout << std::left 
                  << std::setw(10) << ("t = " + std::to_string(t))
                  << std::setw(18) << (std::to_string(mstRes.totalCost) + " m")
                  << std::setw(20) << (std::to_string(dijkstraRes.maxLatency) + " ms")
                  << std::setw(25) << status << "\n";
    }
}