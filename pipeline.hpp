/*
    The Pipeline class is a static class that contains the main logic of the program.
    It is responsible for orchestrating the different stages of the pipeline.
    The pipeline consists of the following stages:
    1. Compute the Minimum Spanning Tree (MST) of the graph
    2. Compute the total weight of the MST
    3. Compute the maximum distance in the MST
    4. Compute the average distance in the MST
*/


#pragma once
#include "MSTFactory.hpp"
#include "graph.hpp"
#include <memory>


/*
    The Pipeline class is a static class that contains the main logic of the program.
*/
class Pipeline {
public:
    static void computeMST(std::shared_ptr<MSTSolver> algo, Graph& graph, std::shared_ptr<Tree>& mst);
    static void computeTotalWeight(std::shared_ptr<Tree>& mst);
    static void computeMaxDistance(std::shared_ptr<Tree>& mst);
    static void computeAverageDistance(std::shared_ptr<Tree>& mst);
    static void computeShortestPath(std::shared_ptr<Tree>& mst, int s, int t);
};
