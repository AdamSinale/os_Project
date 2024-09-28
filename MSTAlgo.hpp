#ifndef MSTFACTORY_HPP
#define MSTFACTORY_HPP

#include "graph.hpp"
#include <vector>
#include <memory>
#include <functional>
#include <tuple>

// Abstract base class for MST algorithms
class MSTSolver {
public:
    virtual std::unique_ptr<Tree> findMST(Graph& g) = 0; // Pure virtual function for finding the MST
    virtual ~MSTSolver() = default;
};

// Prim's Algorithm
class PrimSolver : public MSTSolver {
public:
    std::unique_ptr<Tree> findMST(Graph& g) override;
};

// Kruskal's Algorithm
class KruskalSolver : public MSTSolver {
public:
    std::unique_ptr<Tree> findMST(Graph& g) override;
};

// Borůvka's Algorithm
class BoruvkaSolver : public MSTSolver {
public:
    std::unique_ptr<Tree> findMST(Graph& g) override;
};

// Tarjan's Algorithm
class TarjanSolver : public MSTSolver {
public:
    std::unique_ptr<Tree> findMST(Graph& g) override;
};

// Integer MST Algorithm
class IntegerMSTSolver : public MSTSolver {
public:
    std::unique_ptr<Tree> findMST(Graph& g) override;
};

#endif // MSTFACTORY_HPP
