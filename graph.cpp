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
                std::cout << *vertices[i] << " --("<<ns[j].second<<")-> " << *neighbor << std::endl;
            } else {
                std::cout << *vertices[i] << " -> [expired]" << std::endl;
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

void Tree::dfs(shared_ptr<Vertex> v,shared_ptr<Vertex> p,int d,shared_ptr<Vertex>& fv,int& md,int& sd){
    if(d > md) {                                        // if distance until here > max
        md = d;                                         //    update max
        fv = v;
    }
    for (const auto& n : v->getNeighbors()) {           // for vertex's neighbors
        if (n.first.lock() != p) {                      //    already checked, no infinite loop
            sd += d+n.second;                           //    add distance
            dfs(n.first.lock(),v,d+n.second,fv,md,sd);  //    check neighbors
        }
    }
}

void Tree::maxDistance(){
    shared_ptr<Vertex> sv = vertices[0];
    int md = 0;
    int sd = 0;
    dfs(sv, nullptr, 0, sv, md, sd);

    md = 0;
    shared_ptr<Vertex> fv = sv;
    dfs(sv, nullptr, 0, sv, md, sd);

    cout << "Longest distance in MST:" << endl;
    cout << *sv << " --(" << md << ")-> " << *fv << endl;
}

void Tree::avgDistance(){
    int md = 0;
    int sd = 0;
    double cd = 0;
    shared_ptr<Vertex> sv = vertices[0];
    for(auto& v : vertices){
        dfs(v, nullptr, 0, sv, md, sd);
    }
    sd = sd/2;
    for(size_t i=1; i<vertices.size(); i++){ cd += i; }
    cout << "Average distances of MST:" << endl;
    cout << sd/cd << endl;
}