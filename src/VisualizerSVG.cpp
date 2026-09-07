#include "../include/VisualizerSVG.hpp"
#include <fstream>
#include <algorithm>

void VisualizerSVG::exportMSTToSVG(const std::string& filename, const std::vector<Rack>& racks, const std::vector<Edge>& mstEdges, int width, int height) {
    std::ofstream svg(filename);
    if (!svg.is_open()) return;

    // Encontrar limites para escala
    int maxX = 1, maxY = 1;
    for (const auto& r : racks) {
        maxX = std::max(maxX, r.x);
        maxY = std::max(maxY, r.y);
    }

    float scaleX = (width - 100) / static_cast<float>(maxX);
    float scaleY = (height - 100) / static_cast<float>(maxY);

    svg << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"" << width << "\" height=\"" << height << "\" style=\"background-color:#0d1117;\">\n";
    svg << "<style> line { stroke-dasharray: 1000; stroke-dashoffset: 1000; animation: dash 2s linear forwards; } @keyframes dash { to { stroke-dashoffset: 0; } } </style>\n";

    // Desenhar Arestas da MST
    for (const auto& edge : mstEdges) {
        float x1 = 50 + racks[edge.u].x * scaleX;
        float y1 = 50 + racks[edge.u].y * scaleY;
        float x2 = 50 + racks[edge.v].x * scaleX;
        float y2 = 50 + racks[edge.v].y * scaleY;

        svg << "<line x1=\"" << x1 << "\" y1=\"" << y1 << "\" x2=\"" << x2 << "\" y2=\"" << y2 
            << "\" stroke=\"#58a6ff\" stroke-width=\"2\" />\n";
    }

    // Desenhar Racks (Nós)
    for (const auto& r : racks) {
        float cx = 50 + r.x * scaleX;
        float cy = 50 + r.y * scaleY;

        svg << "<circle cx=\"" << cx << "\" cy=\"" << cy << "\" r=\"6\" fill=\"#238636\" stroke=\"#ffffff\" stroke-width=\"1.5\" />\n";
        svg << "<text x=\"" << cx + 8 << "\" y=\"" << cy + 4 << "\" fill=\"#8b949e\" font-size=\"10\" font-family=\"sans-serif\">" << r.id << "</text>\n";
    }

    svg << "</svg>\n";
    svg.close();
}