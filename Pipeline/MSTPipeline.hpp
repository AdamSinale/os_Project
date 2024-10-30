#ifndef MST_PIPELINE_HPP
#define MST_PIPELINE_HPP

#include "ActiveObject.hpp"
#include "MSTFactory.hpp"
#include "../graph.hpp"
#include <memory>
#include <iostream>

class MSTPipeline {
public:
    MSTPipeline(char algorithmType, Graph& graph) : mstSolver(MSTFactory::MST(algorithmType)), graph(graph){
        // cout << "recieved " << algorithmType << "with graph:" << endl;
        // graph.printGraph();
    }
    void runPipeline();   // Runs the pipeline stages
    void stopPipeline();  // Stops the pipeline

private:
    shared_ptr<MSTSolver> mstSolver; // MST algorithm solver
    Graph& graph;                         // Reference to the original graph
    
    // Pipeline stages as Active Objects
    ActiveObject mstSolveStage;
    ActiveObject printGraphStage;
    ActiveObject totalWeightStage;
    ActiveObject maxDistanceStage;
    ActiveObject avgDistanceStage;
    ActiveObject shortestPathStage;
};

#endif // MST_PIPELINE_HPP
