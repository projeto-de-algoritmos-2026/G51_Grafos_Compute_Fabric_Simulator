#include "../include/VisualizerSVG.hpp"
#include <fstream>
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>

void VisualizerSVG::exportTopologyToSVG(
    const std::string& filename, 
    const std::vector<Rack>& racks, 
    const std::vector<Edge>& mstEdges) 
{
    if (racks.empty()) return;

    std::ofstream svg(filename);
    if (!svg.is_open()) {
        std::cerr << "[ERRO] Não foi possível criar o arquivo SVG: " << filename << "\n";
        return;
    }

    std::unordered_map<int, Rack> rackMap;
    int maxX = 1, maxY = 1;
    for (const auto& r : racks) {
        rackMap[r.id] = r;
        maxX = std::max(maxX, r.x);
        maxY = std::max(maxY, r.y);
    }

    float scaleX = (maxX > 0) ? 800.0f / static_cast<float>(maxX) : 1.0f;
    float scaleY = (maxY > 0) ? 800.0f / static_cast<float>(maxY) : 1.0f;

    svg << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"1000\" height=\"1000\" style=\"background-color:#0d1117;\">\n";

    for (const auto& edge : mstEdges) {
        int u = edge.u;
        int v = edge.v;

        if (rackMap.count(u) && rackMap.count(v)) {
            float x1 = 100 + rackMap[u].x * scaleX;
            float y1 = 100 + rackMap[u].y * scaleY;
            float x2 = 100 + rackMap[v].x * scaleX;
            float y2 = 100 + rackMap[v].y * scaleY;

            svg << "<line x1=\"" << x1 << "\" y1=\"" << y1 << "\" x2=\"" << x2 << "\" y2=\"" << y2 
                << "\" stroke=\"#3fb950\" stroke-width=\"2\" opacity=\"0.7\" />\n";
        }
    }

    for (const auto& r : racks) {
        float cx = 100 + r.x * scaleX;
        float cy = 100 + r.y * scaleY;

        svg << "<circle cx=\"" << cx << "\" cy=\"" << cy << "\" r=\"8\" fill=\"#238636\" stroke=\"#ffffff\" stroke-width=\"1.5\" />\n";
        svg << "<text x=\"" << cx + 11 << "\" y=\"" << cy + 4 << "\" fill=\"#c9d1d9\" font-size=\"11\" font-family=\"sans-serif\">" << r.id << "</text>\n";
    }

    svg << "</svg>\n";
    svg.close();
}

void VisualizerSVG::exportShortestPathTreeToSVG(
    const std::string& filepath, 
    const std::vector<Rack>& racks, 
    const std::vector<int>& parent, 
    int sourceNode) 
{
    if (racks.empty()) return;

    std::ofstream svg(filepath);
    if (!svg.is_open()) {
        std::cerr << "[ERRO] Não foi possível criar o arquivo SVG: " << filepath << "\n";
        return;
    }

    std::unordered_map<int, Rack> rackMap;
    int maxX = 1, maxY = 1;
    for (const auto& r : racks) {
        rackMap[r.id] = r;
        maxX = std::max(maxX, r.x);
        maxY = std::max(maxY, r.y);
    }

    float scaleX = (maxX > 0) ? 800.0f / static_cast<float>(maxX) : 1.0f;
    float scaleY = (maxY > 0) ? 800.0f / static_cast<float>(maxY) : 1.0f;

    svg << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"1000\" height=\"1000\" style=\"background-color:#0d1117;\">\n";

    for (size_t v = 0; v < parent.size(); ++v) {
        int u = parent[v];
        if (u != -1 && rackMap.count(u) && rackMap.count(v)) {
            float x1 = 100 + rackMap[u].x * scaleX;
            float y1 = 100 + rackMap[u].y * scaleY;
            float x2 = 100 + rackMap[v].x * scaleX;
            float y2 = 100 + rackMap[v].y * scaleY;

            svg << "<line x1=\"" << x1 << "\" y1=\"" << y1 << "\" x2=\"" << x2 << "\" y2=\"" << y2 
                << "\" stroke=\"#2f81f7\" stroke-width=\"2.5\" opacity=\"0.8\" />\n";
        }
    }

    for (const auto& r : racks) {
        float cx = 100 + r.x * scaleX;
        float cy = 100 + r.y * scaleY;
        std::string color = (r.id == sourceNode) ? "#d29922" : "#238636";

        svg << "<circle cx=\"" << cx << "\" cy=\"" << cy << "\" r=\"8\" fill=\"" << color << "\" stroke=\"#ffffff\" stroke-width=\"1.5\" />\n";
        svg << "<text x=\"" << cx + 11 << "\" y=\"" << cy + 4 << "\" fill=\"#c9d1d9\" font-size=\"11\" font-family=\"sans-serif\">" << r.id << "</text>\n";
    }

    svg << "</svg>\n";
    svg.close();
}