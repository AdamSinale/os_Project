#include "graph.hpp"

void Graph::addEdge(shared_ptr<Vertex> v, shared_ptr<Vertex> u, int w) {
    if(v->id == u->id){ return; }
    v->addNeighbor(u, w);
    u->addNeighbor(v, w);
}

vector<tuple<shared_ptr<Vertex>, shared_ptr<Vertex>, int>> Graph::getEdges() {
    vector<tuple<shared_ptr<Vertex>, shared_ptr<Vertex>, int>> edges;
    for (const auto& v : vertices) {
        for (const auto& n : v->neighbors) {
            if (v->id < n.first.lock()->id) {  // Use weak_ptr's lock() to get shared_ptr
                edges.push_back(make_tuple(v, n.first.lock(), n.second));
            }
        }
    }
    return edges;
}

void Graph::printGraph(){
    for (size_t i = 0; i < vertices.size(); i++) {
        vector<pair<weak_ptr<Vertex>, int>> ns = vertices[i]->getNeighbors();
        for (size_t j = 0; j < ns.size(); j++) {
            shared_ptr<Vertex> neighbor = ns[j].first.lock();
            if(vertices[i]->id < neighbor->id){ continue; }
            if (neighbor) {
                std::cout << vertices[i]->id << " -"<<ns[j].second<<"> " << neighbor->id << std::endl;
            } else {
                std::cout << vertices[i]->id << " -> [expired]" << std::endl;
            }
        }
    }
}


void Graph::printWeight(){
    int count = 0;
    for (const auto& v : vertices) {
        for (const auto& n : v->neighbors) {
            if (v->id < n.first.lock()->id) {  // Use weak_ptr's lock() to get shared_ptr
                count += n.second;
            }
        }
    }
    cout << "Total weight of MST:" << endl;
    cout << count << endl;
}