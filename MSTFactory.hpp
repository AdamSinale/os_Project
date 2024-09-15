#include "MSTAlgo.hpp"

#include <iostream>
#include <memory>

// Enum for selecting the algorithm type
enum class MSTType { BORUVKA, PRIM, KRUSKAL, TARJAN, INTEGERMST };

class MSTFactory {
public:
    std::unique_ptr<MSTSolver> createMSTSolver(MSTType type) {
        switch(type) {
            case MSTType::BORUVKA:
                return std::make_unique<BoruvkaSolver>();
            case MSTType::PRIM:
                return std::make_unique<PrimSolver>();
            case MSTType::KRUSKAL:
                return std::make_unique<KruskalSolver>();
            case MSTType::TARJAN:
                return std::make_unique<TarjanSolver>();
            case MSTType::INTEGERMST:
                return std::make_unique<IntegerMSTSolver>();
            default:
                return nullptr;
        }
    }
};
