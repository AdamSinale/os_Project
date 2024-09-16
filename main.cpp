#include "graph.hpp"
#include "MSTFactory.hpp"
#include <vector>
#include <iostream>

#define invalid 0
#define valid 1

using std::cin;

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

    vector<Vertex> vertices(n);
    for (int i = 0; i < n; i++) { vertices[i] = Vertex(i); }
    Graph graph(vertices);
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
        }
        if (u == v) { // self loop
            cout << "No inside edge" << endl;
            i--;
        }
        if (vertices[v].hasNeighbor(&vertices[u])) { // multiple edges
            cout << "Edge already exists" << endl;
            i--;
        }
        cout << "Weight:" << endl;
        cin >> w;
        vertices[v].addNeighbor(&vertices[u], w);
    }

    char a;
    cout << "Choose a MST algorithm:" << endl;
    cout << "p - Prim's Algorithm" << endl;
    cout << "t - Tarjan's Algorithm" << endl;
    cout << "k - Kruskal's Algorithm" << endl;
    cout << "b - Boruvka's Algorithm" << endl;
    cout << "i - IntegerMST's Algorithm" << endl;
    cin >> a;
    
    unique_ptr<MSTSolver> algo = MSTFactory::MST(a);
    algo->findMST(graph);
    return 0;
}
