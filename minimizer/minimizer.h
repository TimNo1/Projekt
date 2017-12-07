//
// Created by dario on 02.11.17..
//

#ifndef PROJEKT_MINIMIZER_H
#define PROJEKT_MINIMIZER_H

#include <vector>
#include <string>
#include <unordered_set>

typedef long long hashType;

namespace minimizer {
    struct MinimizerTriple {
        hashType h;
        int position;
        bool rc;

        MinimizerTriple(hashType _h, int _position, bool _rc)
                : h(_h), position(_position), rc(_rc) {}

        inline bool operator<(const MinimizerTriple& other) const {
            return h < other.h;
        }

    };

    // vrati minimizere redom kako se oni nalaze u stringu
    std::vector<MinimizerTriple> computeMinimizers(const std::string& target, int w, int k);

} // namespace minimizer

#endif //PROJEKT_MINIMIZER_H
