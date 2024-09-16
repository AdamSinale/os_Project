
#include "graph.hpp"

#include <iostream>
using std::cout;
using std::endl;

// Abstract base class for MST algorithms
class MSTSolver {
public:
    virtual void findMST(Graph& g) = 0; // Pure virtual function for finding the MST
    virtual ~MSTSolver() = default;
};

class PrimSolver : public MSTSolver {
public:
    void findMST(Graph& g) override {
        cout << "Finding MST using Prim's Algorithm" << endl;
    }
};

class KruskalSolver : public MSTSolver {
public:
    void findMST(Graph& g) override {
        cout << "Finding MST using Kruskal's Algorithm" << endl;
    }
};

class BoruvkaSolver : public MSTSolver {
public:
    void findMST(Graph& g) override {
        cout << "Finding MST using Boruvka's Algorithm" << endl;
    }
};

class TarjanSolver : public MSTSolver {
public:
    void findMST(Graph& g) override {
        cout << "Finding MST using Tarjan's Algorithm" << endl;
    }
};

class IntegerMSTSolver : public MSTSolver {
public:
    void findMST(Graph& g) override {
        cout << "Finding MST using IntegerMST's Algorithm" << endl;
    }
};
