#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <vector>
#include <algorithm>
#include <iostream>

class Vertex {
public:
    int id;
    std::vector<std::pair<Vertex*, int>> neighbors;

    Vertex() {}
    Vertex(int id) : id(id) {}

    void addNeighbor(Vertex* neighbor, int weight);

    void removeNeighbor(Vertex* neighbor);

    bool hasNeighbor(Vertex* neighbor);

};

#endif
