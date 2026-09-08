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
    if (!svg.is_open()) return;

    std::unordered_map<int, Rack> rackMap;
    int minX = 1e9, minY = 1e9, maxX = -1e9, maxY = -1e9;
    for (const auto& r : racks) {
        rackMap[r.id] = r;
        minX = std::min(minX, r.x);
        minY = std::min(minY, r.y);
        maxX = std::max(maxX, r.x);
        maxY = std::max(maxY, r.y);
    }

    // Canvas geometry
    float margin = 80.0f;
    float targetWidth = 1000.0f - 2 * margin;
    float targetHeight = 1000.0f - 2 * margin;

    float rangeX = (maxX > minX) ? static_cast<float>(maxX - minX) : 1.0f;
    float rangeY = (maxY > minY) ? static_cast<float>(maxY - minY) : 1.0f;

    float scaleX = targetWidth / rangeX;
    float scaleY = targetHeight / rangeY;

    // Use uniform scaling to keep aspect ratio intact
    float scale = std::min(scaleX, scaleY);

    svg << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"1000\" height=\"1000\" style=\"background-color:#0d1117;\">\n";

    for (const auto& edge : mstEdges) {
        int u = edge.u;
        int v = edge.v;

        if (rackMap.count(u) && rackMap.count(v)) {
            float x1 = margin + (rackMap[u].x - minX) * scale;
            float y1 = margin + (rackMap[u].y - minY) * scale;
            float x2 = margin + (rackMap[v].x - minX) * scale;
            float y2 = margin + (rackMap[v].y - minY) * scale;

            svg << "<line x1=\"" << x1 << "\" y1=\"" << y1 << "\" x2=\"" << x2 << "\" y2=\"" << y2 
                << "\" stroke=\"#3fb950\" stroke-width=\"2.5\" opacity=\"0.8\" />\n";
        }
    }

    for (const auto& r : racks) {
        float cx = margin + (r.x - minX) * scale;
        float cy = margin + (r.y - minY) * scale;

        svg << "<circle cx=\"" << cx << "\" cy=\"" << cy << "\" r=\"9\" fill=\"#238636\" stroke=\"#ffffff\" stroke-width=\"1.5\" />\n";
        svg << "<text x=\"" << cx + 12 << "\" y=\"" << cy + 4 << "\" fill=\"#c9d1d9\" font-size=\"11\" font-family=\"sans-serif\" font-weight=\"bold\">" << r.id << "</text>\n";
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
    int maxX = 0, maxY = 0;
    for (size_t i = 0; i < racks.size(); ++i) {
        rackMap[racks[i].id] = racks[i];
        maxX = std::max(maxX, racks[i].x);
        maxY = std::max(maxY, racks[i].y);
    }

    // Dynamic scaling logic based on coordinate range
    bool isRawGrid = (maxX <= 20 && maxY <= 20);
    float scaleX = isRawGrid ? (maxX > 0 ? 750.0f / static_cast<float>(maxX) : 1.0f) : 1.0f;
    float scaleY = isRawGrid ? (maxY > 0 ? 750.0f / static_cast<float>(maxY) : 1.0f) : 1.0f;
    float offsetX = isRawGrid ? 100.0f : 0.0f;
    float offsetY = isRawGrid ? 100.0f : 0.0f;

    svg << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"1000\" height=\"1000\" style=\"background-color:#0d1117;\">\n";

    for (size_t v = 0; v < parent.size(); ++v) {
        int u = parent[v];
        if (u != -1) {
            float x1 = 0, y1 = 0, x2 = 0, y2 = 0;
            bool foundU = false, foundV = false;

            // Resolve U coordinates (try ID first, then array index)
            if (rackMap.count(u)) { 
                x1 = offsetX + rackMap[u].x * scaleX; 
                y1 = offsetY + rackMap[u].y * scaleY; 
                foundU = true; 
            } else if (u < static_cast<int>(racks.size())) { 
                x1 = offsetX + racks[u].x * scaleX; 
                y1 = offsetY + racks[u].y * scaleY; 
                foundU = true; 
            }

            // Resolve V coordinates (try ID first, then array index)
            if (rackMap.count(static_cast<int>(v))) { 
                x2 = offsetX + rackMap[v].x * scaleX; 
                y2 = offsetY + rackMap[v].y * scaleY; 
                foundV = true; 
            } else if (v < racks.size()) { 
                x2 = offsetX + racks[v].x * scaleX; 
                y2 = offsetY + racks[v].y * scaleY; 
                foundV = true; 
            }

            if (foundU && foundV) {
                svg << "<line x1=\"" << x1 << "\" y1=\"" << y1 << "\" x2=\"" << x2 << "\" y2=\"" << y2 
                    << "\" stroke=\"#2f81f7\" stroke-width=\"2.5\" opacity=\"0.85\" />\n";
            }
        }
    }

    for (const auto& r : racks) {
        float cx = offsetX + r.x * scaleX;
        float cy = offsetY + r.y * scaleY;
        std::string color = (r.id == sourceNode) ? "#d29922" : "#238636";

        svg << "<circle cx=\"" << cx << "\" cy=\"" << cy << "\" r=\"9\" fill=\"" << color << "\" stroke=\"#ffffff\" stroke-width=\"1.5\" />\n";
        svg << "<text x=\"" << cx + 12 << "\" y=\"" << cy + 4 << "\" fill=\"#c9d1d9\" font-size=\"11\" font-family=\"sans-serif\" font-weight=\"bold\">" << r.id << "</text>\n";
    }

    svg << "</svg>\n";
    svg.close();
}