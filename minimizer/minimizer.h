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

        inline bool operator==(const MinimizerTriple& other) const {
            return position == other.position && rc == other.rc; //naravno ako je ovo dvoje jednako i hashevi su jednaki
        }
    };

    std::vector<MinimizerTriple> computeMinimizers(const std::string& target, int w, int k);

} // namespace minimizer

namespace std {
    template<>
    struct hash<minimizer::MinimizerTriple> {
        size_t operator()(const minimizer::MinimizerTriple &mp) const {
            return (mp.position << 1) + (mp.rc ? 1
                                               : 0); // dva minimizera ce imati isti hash samo ako su na istoj poziciji i oba su rc ili oba nisu rc
        }
    };
} //namespace std

#endif //PROJEKT_MINIMIZER_H
