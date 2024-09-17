#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <tuple>
#include <memory> // For std::shared_ptr
#include "vertex.hpp"

using std::vector;
using std::tuple;
using std::make_tuple;
using std::shared_ptr;

class Graph {
private:
    vector<shared_ptr<Vertex>> vertices;  // Use shared_ptr for vertices

public:
    // Constructor accepts by const reference if you don't need to modify the input vector
    Graph(const vector<shared_ptr<Vertex>>& vs) : vertices(vs) {}

    vector<shared_ptr<Vertex>> getVertices(){ return vertices; }
    void addVertex(shared_ptr<Vertex> v){ vertices.push_back(v); }
    void addEdge(shared_ptr<Vertex> v, shared_ptr<Vertex> u, int w);
    size_t numVertices(){ return vertices.size(); }
    vector<tuple<shared_ptr<Vertex>, shared_ptr<Vertex>, int>> getEdges();
    void printGraph();
};

#endif
