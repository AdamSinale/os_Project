#include "pipeline.hpp"
#include <iostream>  


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
