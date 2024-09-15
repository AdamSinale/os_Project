#include "vertex.hpp"

using namespace std;

void Vertex::addNeighbor(Vertex* neighbor, int weight) {
    if (!hasNeighbor(neighbor)) {
        neighbors.push_back(std::make_pair(neighbor, weight));
    }
}

void Vertex::removeNeighbor(Vertex* neighbor) {
    auto it = std::find_if(neighbors.begin(), neighbors.end(),[neighbor](const std::pair<Vertex*, int>& p) { return p.first == neighbor; });
    if (it != neighbors.end()) {
        neighbors.erase(it);
    }
}

bool Vertex::hasNeighbor(Vertex* neighbor) {
    return std::any_of(neighbors.begin(), neighbors.end(),[neighbor](const std::pair<Vertex*, int>& p) { return p.first == neighbor; });
}