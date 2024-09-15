
#include <iostream>

// Abstract base class for MST algorithms
class MSTSolver {
public:
    virtual void findMST() = 0; // Pure virtual function for finding the MST
    virtual ~MSTSolver() = default;
};

class PrimSolver : public MSTSolver {
public:
    void findMST() override {
        std::cout << "Finding MST using Prim's Algorithm" << std::endl;
    }
};

class KruskalSolver : public MSTSolver {
public:
    void findMST() override {
        std::cout << "Finding MST using Kruskal's Algorithm" << std::endl;
    }
};

class BoruvkaSolver : public MSTSolver {
public:
    void findMST() override {
        std::cout << "Finding MST using Boruvka's Algorithm" << std::endl;
    }
};

class TarjanSolver : public MSTSolver {
public:
    void findMST() override {
        std::cout << "Finding MST using Tarjan's Algorithm" << std::endl;
    }
};

class IntegerMSTSolver : public MSTSolver {
public:
    void findMST() override {
        std::cout << "Finding MST using IntegerMST's Algorithm" << std::endl;
    }
};
