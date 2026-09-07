#ifndef SIMULATOR_HPP
#define SIMULATOR_HPP

#include "Graph.hpp"
#include "Kruskal.hpp"
#include "Dijkstra.hpp"
#include <string>
#include <vector>

class Simulator {
private:
    std::vector<Rack> racks;

public:
    Simulator() = default;

    // Métodos de carregamento de dados
    bool loadRacksFromFile(const std::string& filename);
    bool loadNetworkFromFile(const std::string& filename, Graph& outGraph, int& outSource);

    // Executores das etapas
    void runStage1_Topology();
    void runStage2_Routing(const Graph& graph, int sourceNode);
};

#endif // SIMULATOR_HPP