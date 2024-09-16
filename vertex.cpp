#include "vertex.hpp"

using std::make_pair;
using std::find_if;
using std::any_of;

void Vertex::addNeighbor(Vertex* neighbor, int weight) {
    if (!hasNeighbor(neighbor)) {
        neighbors.push_back(make_pair(neighbor, weight));
    }
}

void Vertex::removeNeighbor(Vertex* neighbor) {
    auto it = find_if(neighbors.begin(), neighbors.end(),[neighbor](const pair<Vertex*, int>& p) { return p.first == neighbor; });
    if (it != neighbors.end()) {
        neighbors.erase(it);
    }
}

bool Vertex::hasNeighbor(Vertex* neighbor) {
    return any_of(neighbors.begin(), neighbors.end(),[neighbor](const pair<Vertex*, int>& p) { return p.first == neighbor; });
}
