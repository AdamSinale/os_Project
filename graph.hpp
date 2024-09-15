#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include "vertex.hpp"

class Graph {
private:
    std::vector<Vertex> vertices;

public:
    Graph() {};
    Graph(std::vector<Vertex>& vs) : vertices(vs) {};
};

#endif
