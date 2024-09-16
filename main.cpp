#include "graph.hpp"
#include "MSTFactory.hpp"
#include <vector>
#include <iostream>
#include <memory> // For std::shared_ptr

#define invalid 0
#define valid 1

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::shared_ptr;
using std::make_shared;

int checkValid(int n, int m) {
    if (n <= 0 || m < 0) { return invalid; }  // Should be 1+ vertices and 0+ edges
    if (m > n * (n - 1)) { return invalid; }  // m should be less than n*(n-1)
    return valid;
}

int main() {
    int n, m;
    cout << "How many vertices and edges?" << endl;
    cin >> n >> m;

    int ans = checkValid(n, m);
    if (ans == invalid) {
        cout << "Invalid Input" << endl;
        return 0;
    }

    vector<shared_ptr<Vertex>> vertices;  // Use shared_ptr to manage Vertex objects
    for (int i = 0; i < n; ++i) {
        vertices.push_back(make_shared<Vertex>(i)); // Create new Vertex using shared_ptr
    }
    Graph graph(vertices);  // Pass vector of shared_ptr to Graph

    cout << "A graph of " << n << " vertices." << endl;
    cout << "Add " << m << " edges:" << endl;
    for (int i = 0; i < m; i++) { // input edges
        int u, v, w;
        cin >> u >> v;
        u--;
        v--;
        if (u < 0 || u >= n || v < 0 || v >= n) { // vertex should be in range
            cout << "Input between 1-n" << endl;
            i--;
            continue;
        }
        if (u == v) { // self loop
            cout << "No inside edge" << endl;
            i--;
            continue;
        }
        if (vertices[v]->hasNeighbor(vertices[u])) { // multiple edges
            cout << "Edge already exists" << endl;
            i--;
            continue;
        }
        cout << "Weight:" << endl;
        cin >> w;
        graph.addEdge(vertices[v], vertices[u], w);
    }

    char a;
    cout << "Choose a MST algorithm:" << endl;
    cout << "p - Prim's Algorithm" << endl;
    cout << "t - Tarjan's Algorithm" << endl;
    cout << "k - Kruskal's Algorithm" << endl;
    cout << "b - Boruvka's Algorithm" << endl;
    cout << "i - IntegerMST's Algorithm" << endl;
    cin >> a;

    shared_ptr<MSTSolver> algo = MSTFactory::MST(a);
    shared_ptr<Graph> mst = algo->findMST(graph);
    mst->printGraph();
    return 0;
}

// 6 7 1 2 2 1 3 1 2 3 3 1 4 5 1 5 3 1 6 1 5 6 1 k