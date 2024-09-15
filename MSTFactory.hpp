#include "MSTAlgo.hpp"

#include <iostream>
#include <memory>
using std::make_unique;

class MSTFactory {
public:
    std::unique_ptr<MSTSolver> MST(char type) {
        switch(type) {
            case 'b':
                return make_unique<BoruvkaSolver>();
            case 'p':
                return make_unique<PrimSolver>();
            case 'k':
                return make_unique<KruskalSolver>();
            case 't':
                return make_unique<TarjanSolver>();
            case 'i':
                return make_unique<IntegerMSTSolver>();
            default:
                return nullptr;
        }
    }
};
