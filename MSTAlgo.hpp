#include "graph.hpp"

#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <numeric>      // for iota
#include <functional>   // for std::function
#include <memory>       // for std::unique_ptr

using std::cout;
using std::endl;
using std::get;
using std::vector;
using std::shared_ptr;
using std::make_shared;

// Abstract base class for MST algorithms
class MSTSolver {
public:
    virtual std::unique_ptr<Graph> findMST(Graph& g) = 0; // Pure virtual function for finding the MST
    virtual ~MSTSolver() = default;
};

class PrimSolver : public MSTSolver {
public:
    std::unique_ptr<Graph> findMST(Graph& g) override {
        cout << "Finding MST using Prim's Algorithm:" << endl;
        // Implement Prim's algorithm
        return nullptr;  // Return a unique_ptr<Graph>
    }
};

class KruskalSolver : public MSTSolver {
public:
    std::unique_ptr<Graph> findMST(Graph& g) override {
        cout << "Finding MST using Kruskal's Algorithm" << endl;
        size_t n = g.numVertices();
        vector<tuple<shared_ptr<Vertex>, shared_ptr<Vertex>, int>> edges = g.getEdges();
        
        std::sort(edges.begin(), edges.end(), [](const tuple<shared_ptr<Vertex>, shared_ptr<Vertex>, int>& a, const tuple<shared_ptr<Vertex>, shared_ptr<Vertex>, int>& b) {
            return get<2>(a) < get<2>(b);
        });
        
        vector<int> scc(n);
        iota(scc.begin(), scc.end(), 0);  // Initialize connected components
        
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

        tuple<shared_ptr<Vertex>, shared_ptr<Vertex>, int> edge = edges[0];

        vector<shared_ptr<Vertex>> vertices;
        for (size_t i = 0; i < n; ++i) {
            vertices.push_back(make_shared<Vertex>(i));
        }
        std::unique_ptr<Graph> tree = std::make_unique<Graph>(vertices);

        int v = get<0>(edge)->id;
        int u = get<1>(edge)->id;
        int w = get<2>(edge);
        tree->addEdge(vertices[v], vertices[u], w);
        unionSets(vertices[v]->id, vertices[u]->id);

        // Process remaining edges
        for (size_t i = 1; i < edges.size(); i++) {
            shared_ptr<Vertex> v = get<0>(edges[i]);
            shared_ptr<Vertex> u = get<1>(edges[i]);
            int w = get<2>(edges[i]);

            if (find(v->id) == find(u->id)) continue;
            unionSets(v->id, u->id);
            tree->addEdge(vertices[v->id], vertices[u->id], w);
        }

        return tree;  // Return the new MST graph
    }
};

class BoruvkaSolver : public MSTSolver {
public:
    std::unique_ptr<Graph> findMST(Graph& g) override {
        cout << "Finding MST using Boruvka's Algorithm" << endl;
        // Implement Boruvka's algorithm
        return nullptr;
    }
};

class TarjanSolver : public MSTSolver {
public:
    std::unique_ptr<Graph> findMST(Graph& g) override {
        cout << "Finding MST using Tarjan's Algorithm" << endl;
        // Implement Tarjan's algorithm
        return nullptr;
    }
};

class IntegerMSTSolver : public MSTSolver {
public:
    std::unique_ptr<Graph> findMST(Graph& g) override {
        cout << "Finding MST using IntegerMST's Algorithm" << endl;
        // Implement IntegerMST algorithm
        return nullptr;
    }
};
