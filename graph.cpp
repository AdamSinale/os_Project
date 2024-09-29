#include "graph.hpp"

void Graph::addEdge(shared_ptr<Vertex> v, shared_ptr<Vertex> u, int w) {
    if(v->id == u->id){ return; }
    v->addNeighbor(u, w);
    u->addNeighbor(v, w);
}
void Graph::removeEdge(shared_ptr<Vertex> v, shared_ptr<Vertex> u) {
    if(v->id == u->id){ return; }
    v->removeNeighbor(u);
    u->removeNeighbor(v);
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

void Graph::shortestPath(int sID, int tID, shared_ptr<Tree>& tree){  // O((V+E)logV)
    int s = sID-1;
    int t = tID-1;
    
    vector<int> dist(numVertices(),INT_MAX);
    vector<int> prev(numVertices(),-1);  // path uses 1+ MST edge -> True
    vector<bool> hasEdge(numVertices(),false);  // path uses 1+ MST edge -> True
    dist[s] = 0;
    // Priority queue to store (distance, vertex) pairs
    using pqElement = tuple<int,int,bool>;
    priority_queue<pqElement,vector<pqElement>,greater<pqElement>> pq;
    pq.push({0, s, false}); 

    while (!pq.empty()) {                                 // for each edge kept  O(m)
        auto [cd,cv,hasMSTedge] = pq.top();               // get current edge
        pq.pop(); 
        if(cv == t && hasMSTedge) break;                  // Target reached - finish
        for(auto &n : vertices[cv]->getNeighbors()){                // for each neighbor  O(n)
            auto nv = n.first.lock()->id;
            int w = n.second;
            int nd = cd + w;
            bool edgeInMST = tree->hasEdge(cv,nv);
            if (nd < dist[nv] || (nd == dist[nv] && hasMSTedge && !hasEdge[nv])){
                dist[nv] = nd;
                prev[nv] = cv;
                hasEdge[nv] = hasMSTedge || edgeInMST;  // update edge 
                pq.push({nd, nv, hasEdge[nv]});
            }
        }
    }
    shared_ptr<Vertex> sv = vertices[s];
    shared_ptr<Vertex> tv = vertices[t];
    if(dist[t] == INT_MAX){ cout << "No path from "<<*sv<<" to "<<*tv<< endl; }
    else {
        cout << "Shortest path from " << *sv << " to " << *tv << " = " << dist[t] << endl;
        cout << "Path: ";
        for (shared_ptr<Vertex> v = tv; v != nullptr; v = vertices[prev[v->id]]) {
            cout << *v << " ";
            if (v == sv) break;
        }
        cout << endl;
    }
}