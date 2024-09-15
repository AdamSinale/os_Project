#include "graph.hpp"
#include <vector>
#include <iostream>

#define invalid 0
#define valid 1

using namespace std;

int checkValid(int n, int m) {
    if (n <= 0 || m < 0) { return invalid; }  // Should be 1+ vertices and 0+ edges
    if (m > n * (n - 1)) { return invalid; }  // m should be less than n*(n-1)
    return valid;
}

int main() {
    int n, m;
    cout << "How many vertices and edges?" << endl;
    std::cin >> n >> m;

    int ans = checkValid(n, m);
    if (ans == invalid) {
        cout << "Invalid Input" << endl;
        return 0;
    }

    vector<Vertex> vertices(n);
    for (int i = 0; i < n; i++) { vertices[i] = Vertex(i); }
    Graph graph(vertices);
    cout << "A graph of " << n << " vertices" << endl;
    cout << "add " << m << " edges:" << endl;

    for (int i = 0; i < m; i++) { // input edges
        int u, v, w;
        cin >> u >> v;
        u--;
        v--;
        if (u < 0 || u >= n || v < 0 || v >= n) { // vertex should be in range
            cout << "Invalid Input" << endl;
            return 0;
        }
        if (u == v) { // self loop
            cout << "Invalid Input" << endl;
            return 0;
        }
        if (vertices[v].hasNeighbor(&vertices[u])) { // multiple edges
            cout << "Invalid Input" << endl;
            return 0;
        }

        cout << "Weight:" << endl;
        cin >> w;
        vertices[v].addNeighbor(&vertices[u], w);
    }

    return 0;
}
