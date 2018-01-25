//
// Created by dario on 12.12.17..
//

#ifndef PROJEKT_LIS_H
#define PROJEKT_LIS_H

#include <minimizer.h>
#include <unordered_map>
#include <minimizerMap.h>

namespace lis {

    struct hashTableElement {
        int sequenceIndex, position;
        bool rc;

        //hashTableElement(int _sequenceIndex, int _position, bool _rc)
        //: sequenceIndex(_sequenceIndex), position(_position), rc(_rc) {}
    };
    std::vector<std::pair<int, bool>> getSimilar(int minimizerIndex, std::vector<minimizer::MinimizerTriple> v1,
                                                 minimizerMap& ht);
} // namespace lis

#endif //PROJEKT_LIS_H
