//
// Created by dario on 12.12.17..
//

#include "lis.h"
#include <algorithm>
#include <iostream>
#include <unordered_map>

namespace {
    int ceilIndex(std::vector<int> &v, int l, int r, int key) {
        while (r-l > 1) {
            int m = l + (r-l)/2;
            if (v[m] >= key)
                r = m;
            else
                l = m;
        }

        return r;
    }

    int lisF(std::vector<int>& v) {
        int n = (int)v.size();
        if (n == 0)
            return 0;

        std::vector<int> tail(n, 0);
        int length = 1; // always points empty slot in tail

        tail[0] = v[0];
        for (int i = 1; i < n; i++) {
            if (v[i] < tail[0])
                // new smallest value
                tail[0] = v[i];
            else if (v[i] > tail[length-1])
                // v[i] extends largest subsequence
                tail[length++] = v[i];
            else
                // v[i] will become end candidate of an existing subsequence or
                // Throw away larger elements in all LIS, to make room for upcoming grater elements than v[i]
                // (and also, v[i] would have already appeared in one of LIS, identify the location and replace it)
                tail[ceilIndex(tail, -1, length-1, v[i])] = v[i];
        }

        return length;
    }


}

namespace lis {
    std::vector<std::pair<int, bool>> getSimilar(int sequenceIndex, std::vector<minimizer::MinimizerTriple> v1,
                                                 minimizerMap& ht){//tu
        std::unordered_map<int, std::vector<int>> seqsForLis;
        std::unordered_map<int, std::vector<int>> seqsForLds;
        for (minimizer::MinimizerTriple mini: v1) {
            for (auto& element: ht.get(mini.h)) {
                if (sequenceIndex <= element.sequenceIndex)
                    continue;
                if (element.rc)
                    seqsForLds[element.sequenceIndex].push_back(element.position * -1);
                else
                    seqsForLis[element.sequenceIndex].push_back(element.position);
            }
        }
        std::vector<std::pair<int, bool>> result;
        int limit = 4;
        for (auto it: seqsForLis) {
            if (lisF(it.second) >= limit)
                result.push_back({it.first, true});
        }
        for (auto it: seqsForLds) {
            if (lisF(it.second) >= limit)
                result.push_back({it.first, false});
        }
        return result;
    }
}
