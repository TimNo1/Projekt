//
// Created by dario on 02.11.17..
//

#include <queue>
#include <assert.h>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include "minimizer.h"

namespace {
    std::map<char, int> baseValue = {{'A', 3}, {'T', 2}, {'G', 1}, {'C', 0}};
    const int BASE = 4;
    std::map<char, char> rcMap = {{'A', 'T'}, {'C', 'G'}, {'T', 'A'}, {'G', 'C'}};

    void push(const minimizer::MinimizerTriple& triple, std::deque<minimizer::MinimizerTriple>& dq) {
        while (!dq.empty() && triple < dq.back()) {
            dq.pop_back();
        }
        dq.push_back(triple);
    }

    void processState(std::deque<minimizer::MinimizerTriple>& dq,
                      std::vector<minimizer::MinimizerTriple>& result, int& lastPositionTaken) {
        assert(!dq.empty());
        minimizer::MinimizerTriple& front = dq.front();
        dq.pop_front();

        if (lastPositionTaken < front.position) {
            result.push_back(front);
            lastPositionTaken = front.position;
        }
        while (!dq.empty() && dq.front().h == front.h) {
            front = dq.front();
            dq.pop_front();
            if (lastPositionTaken < front.position) {
                result.push_back(front);
                lastPositionTaken = front.position;
            }
        }
        dq.push_front(front);
    }

    void pop(int position, std::deque<minimizer::MinimizerTriple>& dq) {
        while (!dq.empty() && dq.front().position == position)
            dq.pop_front();
    }



} // namespace

namespace minimizer {

    std::vector<MinimizerTriple> computeMinimizers(const std::string &target, int w, int k) {
        std::vector<MinimizerTriple> retVec;
        int n = target.size();

        if (n < k) {
            return retVec; // ne postoji ni jedan kmer
        }

        if (n < k + w - 1) {//ne postoji ni jedan window od w kmera
            w = n - k + 1; // smanji velicinu trazenog prozora na najvise sta mozes, da se nadje barem jedan minimizer
        }


        std::deque<MinimizerTriple> dq;

        hashType lastPower = 1;
        for (int i = 0; i < k - 1; i++)
            lastPower *= BASE;

        hashType tmpHash = 0;
        hashType tmpRcHash = 0;

        hashType tmpPot = 1;
        for (int i = 0; i < k; i++) {
            tmpHash *= BASE;
            tmpHash += baseValue[target[i]];
            tmpRcHash += tmpPot * baseValue[rcMap[target[i]]];
            tmpPot *= BASE;
        }

        // queue s maksimumom algoritam
        for (int i = 0; i < w; i++) {
            MinimizerTriple mp1 = MinimizerTriple(tmpHash, i, false);
            MinimizerTriple mp2 = MinimizerTriple(tmpRcHash, i, true);
            push(mp1, dq);
            push(mp2, dq);
            tmpHash -= lastPower * baseValue[target[i]];
            tmpHash *= BASE;
            tmpHash += baseValue[target[i + k]];

            tmpRcHash -= baseValue[rcMap[target[i]]];
            tmpRcHash /= BASE;
            tmpRcHash += lastPower * baseValue[rcMap[target[i + k]]];
        }


        int lastPositionTaken = -1;

        processState(dq, retVec, lastPositionTaken);

        for (int i = w; i < n - k + 1; i++) {
            pop(i - w, dq);
            MinimizerTriple mp1 = MinimizerTriple(tmpHash, i, false);
            MinimizerTriple mp2 = MinimizerTriple(tmpRcHash, i, true);
            push(mp1, dq);
            push(mp2, dq);
            processState(dq, retVec, lastPositionTaken);

            tmpHash -= lastPower * baseValue[target[i]];
            tmpHash *= BASE;
            tmpHash += baseValue[target[i + k]];

            tmpRcHash -= baseValue[rcMap[target[i]]];
            tmpRcHash /= BASE;
            tmpRcHash += lastPower * baseValue[rcMap[target[i + k]]];
        }

        return retVec;
    }




} // namespace minimizer