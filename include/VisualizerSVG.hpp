#ifndef VISUALIZERSVG_HPP
#define VISUALIZERSVG_HPP

#include "Graph.hpp"
#include "Kruskal.hpp"
#include <string>
#include <vector>

class VisualizerSVG {
public:
    // Exporta a topologia e as conexões da MST para um arquivo .svg
    static void exportMSTToSVG(const std::string& filename, const std::vector<Rack>& racks, const std::vector<Edge>& mstEdges, int width = 1000, int height = 1000);
};

#endif // VISUALIZERSVG_HPP