#include "vertex.hpp"

using std::make_pair;
using std::find_if;
using std::any_of;
using std::shared_ptr;
using std::weak_ptr;

void Vertex::addNeighbor(shared_ptr<Vertex> neighbor, int weight) {
    if (!hasNeighbor(neighbor)) {
        neighbors.push_back(make_pair(weak_ptr<Vertex>(neighbor), weight)); // Store weak_ptr
    }
}

void Vertex::removeNeighbor(shared_ptr<Vertex> neighbor) {
    auto it = find_if(neighbors.begin(), neighbors.end(),[neighbor](const pair<weak_ptr<Vertex>, int>& p) { 
        return p.first.lock() == neighbor; // lock weak_ptr to get shared_ptr for comparison
    });
    if (it != neighbors.end()) {
        neighbors.erase(it);
    }
}

bool Vertex::hasNeighbor(shared_ptr<Vertex> neighbor) {
    return any_of(neighbors.begin(), neighbors.end(),[neighbor](const pair<weak_ptr<Vertex>, int>& p) { 
        return p.first.lock() == neighbor; // lock weak_ptr to compare with shared_ptr
    });
}
