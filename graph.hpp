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

#include <climits> // For INT_MAX
#include <queue>
#include <unordered_map> 
using std::unordered_map;
using std::priority_queue;
using std::greater;

class Tree;

class Graph {
protected:
    vector<shared_ptr<Vertex>> vertices;  // Use shared_ptr for vertices

public:
    Graph() { vertices = {}; }
    Graph(const vector<shared_ptr<Vertex>>& vs) : vertices(vs) {}
    void addVertex(shared_ptr<Vertex> v){ vertices.push_back(v); }
    void addEdge(shared_ptr<Vertex> v, shared_ptr<Vertex> u, int w);
    void removeEdge(shared_ptr<Vertex> v, shared_ptr<Vertex> u);
    size_t numVertices(){ return vertices.size(); }
    vector<tuple<shared_ptr<Vertex>, shared_ptr<Vertex>, int>> getEdges();
    vector<shared_ptr<Vertex>> getVertices(){ return vertices; }
    shared_ptr<Vertex> getVertex(int id){ return vertices[id]; }
    bool hasEdge(int vID, int uID){ return vertices[vID]->hasNeighbor(vertices[uID]); }
    void printGraph();
    void printWeight();
    void shortestPath(int sID, int tID, shared_ptr<Tree>& tree);
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
