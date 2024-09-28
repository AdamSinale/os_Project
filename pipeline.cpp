#include "pipeline.hpp"
#include <iostream>  

/*
    The Pipeline class is a static class that contains the main logic of the program.
    It is responsible for orchestrating the different stages of the pipeline.
    The pipeline consists of the following stages:
    1. Compute the Minimum Spanning Tree (MST) of the graph
    2. Compute the total weight of the MST
    3. Compute the maximum distance in the MST
    4. Compute the average distance in the MST
*/

void Pipeline::computeMST(std::shared_ptr<MSTSolver> algo, Graph& graph, std::shared_ptr<Tree>& mst) {
    mst = algo->findMST(graph);
}

void Pipeline::computeTotalWeight(std::shared_ptr<Tree>& mst) {
    mst->printWeight();
}

void Pipeline::computeMaxDistance(std::shared_ptr<Tree>& mst) {
    mst->maxDistance();
}

void Pipeline::computeAverageDistance(std::shared_ptr<Tree>& mst) {
    mst->avgDistance();
}

void Pipeline::computeShortestPath(std::shared_ptr<Tree>& mst, int s, int t) {
    mst->shortestPath(s, t, mst);
}
