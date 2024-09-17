#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <vector>
#include <memory> // For std::shared_ptr and std::weak_ptr
#include <algorithm>
#include <iostream>

using std::vector;
using std::pair;
using std::shared_ptr;
using std::weak_ptr;

class Vertex {
public:
    int id;
    vector<pair<weak_ptr<Vertex>, int>> neighbors; // Use weak_ptr to avoid ownership cycles

    Vertex() {}
    Vertex(int id) : id(id) {}

    int getID(){ return id; }
    
    // Add neighbor to the vertex
    void addNeighbor(shared_ptr<Vertex> neighbor, int weight);

    // Remove neighbor from the vertex
    void removeNeighbor(shared_ptr<Vertex> neighbor);

    // Check if the vertex has the given neighbor
    bool hasNeighbor(shared_ptr<Vertex> neighbor);

    vector<pair<weak_ptr<Vertex>, int>> getNeighbors() { return neighbors; }
};

#endif
