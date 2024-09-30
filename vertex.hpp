/*
    * This file contains the Vertex class, which represents a vertex in a graph.
    * Each vertex has an ID and a list of neighbors, each with an associated weight.
    * The neighbors are stored as a vector of pairs, where the first element is a weak_ptr to the neighbor vertex
    * and the second element is the weight of the edge connecting the two vertices.
    * The class also contains methods to add, remove, and check for neighbors.
*/



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
    vector<pair<weak_ptr<Vertex>, int>> getNeighbors() { return neighbors; }
    
    void addNeighbor(shared_ptr<Vertex> neighbor, int weight);
    void removeNeighbor(shared_ptr<Vertex> neighbor);
    bool hasNeighbor(shared_ptr<Vertex> neighbor);

    bool operator==(Vertex o){ return id == o.id; }
    bool operator!=(Vertex o){ return id != o.id; }
    friend std::ostream& operator<<(std::ostream& os, const Vertex& v){
        os << v.id+1;
        return os;
    }
};

#endif
