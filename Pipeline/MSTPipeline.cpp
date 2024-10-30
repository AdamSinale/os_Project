#include "MSTPipeline.hpp"


void MSTPipeline::runPipeline() {
    mstSolveStage.submitTask([this]() {
        cout << "running PL mstSolver" << endl;
        shared_ptr<Tree> mstree  = mstSolver->findMST(graph);

        printGraphStage.submitTask([this, mstree]() {
            cout << "running PL printGraph" << endl;
            mstree->printGraph();

            totalWeightStage.submitTask([this, mstree]() {
                cout << "running PL totalWeight" << endl;
                int totalWeight = mstree->printWeight();

                maxDistanceStage.submitTask([this, mstree]() {
                    int maxDistance = mstree->maxDistance();

                    avgDistanceStage.submitTask([this, mstree]() {
                        int avgDistance = mstree->avgDistance();

                        shortestPathStage.submitTask([this, mstree]() {
                            vector<shared_ptr<Vertex>> sp = graph.shortestPath(1,2,mstree);

                        });
                    });
                });
            });
        });
    });
}

void MSTPipeline::stopPipeline() {
    mstSolveStage.stop();
    printGraphStage.stop();
    totalWeightStage.stop();
    maxDistanceStage.stop();
    avgDistanceStage.stop();
    shortestPathStage.stop();
}
