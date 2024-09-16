#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <vector>
#include <algorithm>
#include <iostream>

using std::vector;
using std::pair;

class Vertex {
public:
    int id;
    vector<pair<Vertex*, int>> neighbors;

    Vertex() {}
    Vertex(int id) : id(id) {}

    void addNeighbor(Vertex* neighbor, int weight);

    void removeNeighbor(Vertex* neighbor);

    bool hasNeighbor(Vertex* neighbor);

    vector<pair<Vertex*, int>> getNeighbors(){ return neighbors; }
};

#endif
