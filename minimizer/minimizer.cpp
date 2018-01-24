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
    std::unordered_map<minimizer::hashType, int> hashCnt;
    std::map<char, int> baseValue = {{'A', 3}, {'T', 2}, {'G', 1}, {'C', 0}};
    const int BASE = 4;
    std::map<char, char> rcMap = {{'A', 'T'}, {'C', 'G'}, {'T', 'A'}, {'G', 'C'}};

    void push(const minimizer::MinimizerTriple& triple, std::deque<minimizer::MinimizerTriple>& dq, bool maximizer) {
        if(maximizer){
            while (!dq.empty() && triple > dq.back()) {
                dq.pop_back();
            }
        }else{
            while (!dq.empty() && triple < dq.back()) {
              dq.pop_back();
            }
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
            hashCnt[front.h]++;
            lastPositionTaken = front.position;
        }
        while (!dq.empty() && dq.front().h == front.h) {
            front = dq.front();
            dq.pop_front();
            if (lastPositionTaken < front.position) {
                result.push_back(front);
                hashCnt[front.h]++;
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

    std::vector<MinimizerTriple> mergeVectorsByPosition(std::vector<MinimizerTriple> v1,std::vector<MinimizerTriple> v2){
        if(v1.empty())
            return v2;
        if(v2.empty())
            return v1;
        std::vector<MinimizerTriple> retVec;
        int i = 0;
        int j = 0;
        int m=v1.size();
        int n=v2.size();
        while (i < m && j < n) {
            if (v1[i].position <= v2[j].position) {
                retVec.push_back(v1[i]);
                i++;
            } else {
                retVec.push_back(v2[j]);
                j++;
            }
        }
        if (i < m)
            for (int p = i; p < m; p++)
                retVec.push_back(v1[p]);
         else
            for (int p = j; p < n; p++)
                retVec.push_back(v2[p]);
        return retVec;
    }

    std::vector<MinimizerTriple> computeMinimizers(const std::string target, int w, int k) {
        std::vector<MinimizerTriple> retVec, vecMin, vecMax;
        int n = target.size();

        const char* target1 = target.c_str();
        
        if (n < k) {
            return retVec; // ne postoji ni jedan kmer
        }

        if (n < k + w - 1) {//ne postoji ni jedan window od w kmera
            w = n - k + 1; // smanji velicinu trazenog prozora na najvise sta mozes, da se nadje barem jedan minimizer
        }


        std::deque<MinimizerTriple> dqMin,dqMax;

        hashType lastPower = 1;
        hashType tmpHash = 0;
        hashType tmpRcHash = 0;
        hashType tmpPot = 1;
        int lastPositionTaken = -1;

        for (int i = 0; i < k - 1; i++)
            lastPower *= BASE;

        for (int i = 0; i < k; i++) {
            tmpHash *= BASE;
            tmpHash += baseValue[target1[i]];
            tmpRcHash += tmpPot * baseValue[rcMap[target1[i]]];
            tmpPot *= BASE;
        }

        // queue s maksimumom algoritam
        for (int i = 0; i < w; i++) {
            MinimizerTriple mp1 = MinimizerTriple(tmpHash, i, false);
            MinimizerTriple mp2 = MinimizerTriple(tmpRcHash, i, true);
            push(mp1, dqMin, false);
            push(mp2, dqMin, false);
            tmpHash -= lastPower * baseValue[target1[i]];
            tmpHash *= BASE;
            tmpHash += baseValue[target1[i + k]];

            tmpRcHash -= baseValue[rcMap[target1[i]]];
            tmpRcHash /= BASE;
            tmpRcHash += lastPower * baseValue[rcMap[target1[i + k]]];
        }

        processState(dqMin, vecMin, lastPositionTaken);

        for (int i = w; i < n - k + 1; i++) {
            pop(i - w, dqMin);
            MinimizerTriple mp1 = MinimizerTriple(tmpHash, i, false);
            MinimizerTriple mp2 = MinimizerTriple(tmpRcHash, i, true);
            push(mp1, dqMin, false);
            push(mp2, dqMin, false);
            processState(dqMin, vecMin, lastPositionTaken);

            tmpHash -= lastPower * baseValue[target1[i]];
            tmpHash *= BASE;
            tmpHash += baseValue[target1[i + k]];

            tmpRcHash -= baseValue[rcMap[target1[i]]];
            tmpRcHash /= BASE;
            tmpRcHash += lastPower * baseValue[rcMap[target1[i + k]]];
        }
        //od tu

        lastPower = 1;
        tmpHash = 0;
        tmpRcHash = 0;
        tmpPot = 1;
        lastPositionTaken = -1;

        for (int i = 0; i < k - 1; i++)
            lastPower *= BASE;

        for (int i = 0; i < k; i++) {
            tmpHash *= BASE;
            tmpHash += baseValue[target1[i]];
            tmpRcHash += tmpPot * baseValue[rcMap[target1[i]]];
            tmpPot *= BASE;
        }

        // queue s maksimumom algoritam
        for (int i = 0; i < w; i++) {
            MinimizerTriple mp1 = MinimizerTriple(tmpHash, i, false);
            MinimizerTriple mp2 = MinimizerTriple(tmpRcHash, i, true);
            push(mp1, dqMax, true);
            push(mp2, dqMax, true);
            tmpHash -= lastPower * baseValue[target1[i]];
            tmpHash *= BASE;
            tmpHash += baseValue[target1[i + k]];

            tmpRcHash -= baseValue[rcMap[target1[i]]];
            tmpRcHash /= BASE;
            tmpRcHash += lastPower * baseValue[rcMap[target1[i + k]]];
        }

        processState(dqMax, vecMax, lastPositionTaken);

        for (int i = w; i < n - k + 1; i++) {
            pop(i - w, dqMax);
            MinimizerTriple mp1 = MinimizerTriple(tmpHash, i, false);
            MinimizerTriple mp2 = MinimizerTriple(tmpRcHash, i, true);
            push(mp1, dqMax, true);
            push(mp2, dqMax, true);
            processState(dqMax, vecMax, lastPositionTaken);

            tmpHash -= lastPower * baseValue[target1[i]];
            tmpHash *= BASE;
            tmpHash += baseValue[target1[i + k]];

            tmpRcHash -= baseValue[rcMap[target1[i]]];
            tmpRcHash /= BASE;
            tmpRcHash += lastPower * baseValue[rcMap[target1[i + k]]];
        }

        //do tu
        retVec=mergeVectorsByPosition(vecMin,vecMax);
        return retVec;
    }

    std::vector<minimizer::MinimizerTriple> reduceMinimizers(std::vector<MinimizerTriple>& minimizers) {
        std::vector<MinimizerTriple> ret;
        for (MinimizerTriple mini: minimizers) {
            if (hashCnt[mini.h] < 34)
                ret.push_back(mini);
        }
        return ret;
    }




} // namespace minimizer
