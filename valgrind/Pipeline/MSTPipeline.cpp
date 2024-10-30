#include "MSTPipeline.hpp"


MSTPipeline::MSTPipeline(char algorithmType, Graph& graph)
    : mstSolver(MSTFactory::MST(algorithmType)), graph(graph) {
    // 
    // 
}

void MSTPipeline::runPipeline() {
    totalWeightStage.submitTask([this]() {
        // Calculate the total weight of all edges in the graph
        int totalWeight = 0;
        for (const auto& edge : graph.getEdges()) {
            totalWeight += std::get<2>(edge);  // Sum edge weights directly from the graph
        }
        std::cout << "Total Weight: " << totalWeight << std::endl;

        maxDistanceStage.submitTask([this, totalWeight]() {
            // Calculate the maximum edge weight in the graph
            int maxDistance = 0;
            for (const auto& edge : graph.getEdges()) {
                maxDistance = std::max(maxDistance, std::get<2>(edge));
            }
            std::cout << "Max Distance: " << maxDistance << std::endl;

            avgDistanceStage.submitTask([this, maxDistance]() {
                // Calculate the average distance (average edge weight)
                int totalWeight = 0;
                int edgeCount = 0;
                for (const auto& edge : graph.getEdges()) {
                    totalWeight += std::get<2>(edge);
                    ++edgeCount;
                }
                double avgDistance = edgeCount == 0 ? 0 : static_cast<double>(totalWeight) / edgeCount;
                std::cout << "Average Distance: " << avgDistance << std::endl;
            });
        });
    });
}

void MSTPipeline::stopPipeline() {
    totalWeightStage.stop();
    maxDistanceStage.stop();
    avgDistanceStage.stop();
}
