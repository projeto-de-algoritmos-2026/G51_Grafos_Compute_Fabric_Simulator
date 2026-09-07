#ifndef VISUALIZERSVG_HPP
#define VISUALIZERSVG_HPP

#include "Graph.hpp"
#include "Kruskal.hpp"
#include <string>
#include <vector>

class VisualizerSVG {
public:
    static void exportTopologyToSVG(const std::string& filename, const std::vector<Rack>& racks, const std::vector<Edge>& mstEdges);

    // Novo método: Exporta a Árvore de Roteamento do Dijkstra a cada snapshot
    static void exportShortestPathTreeToSVG(
        const std::string& filepath, 
        const std::vector<Rack>& racks, 
        const std::vector<int>& parent, 
        int sourceNode
    );
};

#endif // VISUALIZERSVG_HPP