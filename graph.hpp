#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <tuple>
#include <memory> // For std::shared_ptr
#include "vertex.hpp"
using std::vector;
using std::tuple;
using std::cout;
using std::endl;
using std::make_tuple;
using std::shared_ptr;

class Graph {
protected:
    vector<shared_ptr<Vertex>> vertices;  // Use shared_ptr for vertices

public:
    Graph(const vector<shared_ptr<Vertex>>& vs) : vertices(vs) {}
    vector<shared_ptr<Vertex>> getVertices(){ return vertices; }
    void addVertex(shared_ptr<Vertex> v){ vertices.push_back(v); }
    void addEdge(shared_ptr<Vertex> v, shared_ptr<Vertex> u, int w);
    size_t numVertices(){ return vertices.size(); }
    vector<tuple<shared_ptr<Vertex>, shared_ptr<Vertex>, int>> getEdges();
    void printGraph();
    void printWeight();
};


class Tree : public Graph {
private:
    void dfs(shared_ptr<Vertex> v,shared_ptr<Vertex> p,int d,shared_ptr<Vertex>& fv,int& md,int& sd);
public:
    Tree(const vector<shared_ptr<Vertex>>& vs) : Graph(vs) {}
    void maxDistance();
    void avgDistance();
};

#endif
