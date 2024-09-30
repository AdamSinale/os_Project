/*
    This file contains the implementation of the MSTAlgo class and its subclasses.
    The MSTAlgo class is an abstract class that defines the interface for all MST algorithms.
    The subclasses implement the Prim, Kruskal, Boruvka, Tarjan, and Integer MST algorithms.    
*/


#include "MSTAlgo.hpp"
#include <queue>
#include <limits>
#include <numeric> // for iota
#include <algorithm> // for sort
#include <functional> // for std::function


using std::cout;
using std::endl;
using std::get;
using std::vector;
using std::shared_ptr;
using std::unique_ptr;
using std::make_shared;
using std::make_unique;
using std::priority_queue;
using std::numeric_limits;
using std::greater;

/*
    Find MST using Prim's algorithm.
*/
std::unique_ptr<Tree> PrimSolver::findMST(Graph& g) {
    cout << "Finding MST using Prim's Algorithm:" << endl;
    size_t n = g.numVertices();
    vector<shared_ptr<Vertex>> ogVs = g.getVertices();
    
    priority_queue<std::tuple<int, int, int>, vector<std::tuple<int, int, int>>, greater<std::tuple<int, int, int>>> pq;
    vector<int> key(n, numeric_limits<int>::max());
    vector<bool> inMST(n, false);

    vector<shared_ptr<Vertex>> vertices;
    for (size_t i = 0; i < n; ++i) {
        vertices.push_back(make_shared<Vertex>(i));
    }
    unique_ptr<Tree> tree = make_unique<Tree>(vertices);

    key[0] = 0;
    pq.push(std::make_tuple(0, 0, 0));

    while (!pq.empty()) {
        auto [weight, u, v] = pq.top();
        pq.pop();

        if (inMST[v]) continue;
        inMST[v] = true;
        tree->addEdge(vertices[u], vertices[v], weight);

        for (auto& edge : ogVs[v]->getNeighbors()) {
            int n = edge.first.lock()->getID();
            int w = edge.second;
            if (!inMST[n] && key[n] > w) {
                key[n] = w;
                pq.push(std::make_tuple(w, v, n));
            }
        }
    }
    return tree;
}


/*
    Find MST using Kruskal's algorithm.
*/
std::unique_ptr<Tree> KruskalSolver::findMST(Graph& g) {
    cout << "Finding MST using Kruskal's Algorithm" << endl;
    size_t n = g.numVertices();
    vector<std::tuple<shared_ptr<Vertex>, shared_ptr<Vertex>, int>> edges = g.getEdges();

    std::sort(edges.begin(), edges.end(), [](const std::tuple<shared_ptr<Vertex>, shared_ptr<Vertex>, int>& a, const std::tuple<shared_ptr<Vertex>, shared_ptr<Vertex>, int>& b) {
        return get<2>(a) < get<2>(b);
    });

    vector<int> scc(n);
    std::iota(scc.begin(), scc.end(), 0);

    std::function<int(int)> find = [&](int v) {
        if (scc[v] != v) {
            scc[v] = find(scc[v]);
        }
        return scc[v];
    };

    auto unionSets = [&](int u, int v) {
        int rootU = find(u);
        int rootV = find(v);
        if (rootU != rootV) {
            scc[rootV] = rootU;
        }
    };

    vector<shared_ptr<Vertex>> vertices;
    for (size_t i = 0; i < n; ++i) {
        vertices.push_back(make_shared<Vertex>(i));
    }
    unique_ptr<Tree> tree = make_unique<Tree>(vertices);

    for (const auto& edge : edges) {
        shared_ptr<Vertex> v = get<0>(edge);
        shared_ptr<Vertex> u = get<1>(edge);
        int w = get<2>(edge);

        if (find(v->getID()) != find(u->getID())) {
            unionSets(v->getID(), u->getID());
            tree->addEdge(vertices[v->getID()], vertices[u->getID()], w);
        }
    }

    return tree;
}


/*
    Find MST using Boruvka's algorithm.
*/
std::unique_ptr<Tree> BoruvkaSolver::findMST(Graph& g) {
    cout << "Finding MST using Boruvka's Algorithm" << endl;
    size_t n = g.numVertices();
    vector<shared_ptr<Vertex>> vertices;
    for (size_t i = 0; i < n; ++i) {
        vertices.push_back(make_shared<Vertex>(i));
    }
    unique_ptr<Tree> tree = make_unique<Tree>(vertices);

    vector<int> component(n);
    std::iota(component.begin(), component.end(), 0); // Start each vertex as its own component

    int numComponents = n;
    while (numComponents > 1) {
        // Find the cheapest edge for each component
        vector<std::tuple<int, int, int>> cheapest(n, std::make_tuple(-1, -1, numeric_limits<int>::max()));

        for (const auto& edge : g.getEdges()) {
            int u = get<0>(edge)->getID();
            int v = get<1>(edge)->getID();
            int weight = get<2>(edge);
            if (component[u] != component[v]) { // Different components
                if (weight < std::get<2>(cheapest[component[u]])) {
                    cheapest[component[u]] = std::make_tuple(u, v, weight);
                }
                if (weight < std::get<2>(cheapest[component[v]])) {
                    cheapest[component[v]] = std::make_tuple(v, u, weight);
                }
            }
        }

        // Add the cheapest edges to the MST and merge components
        for (size_t i = 0; i < n; ++i) {
            if (std::get<0>(cheapest[i]) != -1) {
                int u = std::get<0>(cheapest[i]);
                int v = std::get<1>(cheapest[i]);
                int weight = std::get<2>(cheapest[i]);

                if (component[u] != component[v]) {
                    tree->addEdge(vertices[u], vertices[v], weight);
                    int oldComponent = component[v];
                    int newComponent = component[u];
                    for (size_t j = 0; j < n; ++j) {
                        if (component[j] == oldComponent) {
                            component[j] = newComponent;
                        }
                    }
                    numComponents--;
                }
            }
        }
    }

    return tree;
}

/*
    Find MST using Tarjan's algorithm.
*/
std::unique_ptr<Tree> TarjanSolver::findMST(Graph& g) {
    cout << "Finding MST using Tarjan's Algorithm (MST via Union-Find)" << endl;

    size_t n = g.numVertices();
    vector<std::tuple<shared_ptr<Vertex>, shared_ptr<Vertex>, int>> edges = g.getEdges();
    
    // Sort edges by weight (in ascending order)
    std::sort(edges.begin(), edges.end(), [](const std::tuple<shared_ptr<Vertex>, shared_ptr<Vertex>, int>& a, const std::tuple<shared_ptr<Vertex>, shared_ptr<Vertex>, int>& b) {
        return get<2>(a) < get<2>(b);
    });

    // Union-Find (Disjoint Set Union) data structures
    vector<int> parent(n), rank(n, 0);
    std::iota(parent.begin(), parent.end(), 0); // Initialize parent array

    // Helper function to find the root of a set
    std::function<int(int)> findSet = [&](int u) {
        if (u != parent[u]) {
            parent[u] = findSet(parent[u]); // Path compression
        }
        return parent[u];
    };

    // Union by rank
    auto unionSets = [&](int u, int v) {
        u = findSet(u);
        v = findSet(v);
        if (u != v) {
            if (rank[u] < rank[v]) {
                parent[u] = v;
            } else if (rank[u] > rank[v]) {
                parent[v] = u;
            } else {
                parent[v] = u;
                rank[u]++;
            }
        }
    };

    vector<shared_ptr<Vertex>> vertices;
    for (size_t i = 0; i < n; ++i) {
        vertices.push_back(make_shared<Vertex>(i));
    }

    unique_ptr<Tree> tree = make_unique<Tree>(vertices);

    // Iterate over sorted edges
    for (const auto& edge : edges) {
        shared_ptr<Vertex> u = get<0>(edge);
        shared_ptr<Vertex> v = get<1>(edge);
        int w = get<2>(edge);

        // Check if u and v belong to different sets (i.e., not yet connected)
        if (findSet(u->getID()) != findSet(v->getID())) {
            // Union the sets and add the edge to the MST
            unionSets(u->getID(), v->getID());
            tree->addEdge(vertices[u->getID()], vertices[v->getID()], w);
        }
    }

    return tree;
}


/*
    Find Integer MST.
*/
std::unique_ptr<Tree> IntegerMSTSolver::findMST(Graph& g) {
    cout << "Finding MST using Integer MST Algorithm (Prim's Algorithm for Integer Weights)" << endl;

    size_t n = g.numVertices();
    vector<shared_ptr<Vertex>> ogVs = g.getVertices();
    
    // Priority queue to select the minimum edge at each step
    priority_queue<std::tuple<int, int, int>, vector<std::tuple<int, int, int>>, greater<std::tuple<int, int, int>>> pq;
    vector<int> key(n, numeric_limits<int>::max());
    vector<bool> inMST(n, false);

    vector<shared_ptr<Vertex>> vertices;
    for (size_t i = 0; i < n; ++i) {
        vertices.push_back(make_shared<Vertex>(i));
    }
    unique_ptr<Tree> tree = make_unique<Tree>(vertices);

    key[0] = 0;  // Start from vertex 0
    pq.push(std::make_tuple(0, 0, 0));  // Push (weight, u, v) where weight = 0, u = 0

    while (!pq.empty()) {
        auto [weight, u, v] = pq.top();
        pq.pop();

        // If vertex v is already in the MST, skip
        if (inMST[v]) continue;
        inMST[v] = true;  // Add vertex v to the MST

        // Add the edge (u, v) to the MST, with the given weight
        if (u != v) {  // Avoid adding the initial (0, 0) edge
            tree->addEdge(vertices[u], vertices[v], weight);
        }

        // Explore all neighbors of vertex v
        for (const auto& edge : ogVs[v]->getNeighbors()) {
            int neighbor = edge.first.lock()->getID();  // Get neighbor's ID
            int w = edge.second;  // Edge weight

            if (!inMST[neighbor] && w < key[neighbor]) {
                key[neighbor] = w;  // Update the key (minimum weight)
                pq.push(std::make_tuple(w, v, neighbor));  // Push (weight, v, neighbor) to the priority queue
            }
        }
    }

    return tree;
}
