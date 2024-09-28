#pragma once
#include "MSTFactory.hpp"
#include "graph.hpp"
#include <memory>

class Pipeline {
public:
    static void computeMST(std::shared_ptr<MSTSolver> algo, Graph& graph, std::shared_ptr<Tree>& mst);
    static void computeTotalWeight(std::shared_ptr<Tree>& mst);
    static void computeMaxDistance(std::shared_ptr<Tree>& mst);
    static void computeAverageDistance(std::shared_ptr<Tree>& mst);
    static void computeShortestPath(std::shared_ptr<Tree>& mst, int s, int t);
};
