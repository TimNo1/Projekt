//
// Created by dario on 02.11.17..
//

#include <queue>
#include <assert.h>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include "minimizer.h"
#include "DnaSequence.h"

namespace {

    int mulMod(int a, int b, int MOD) {
        return (a * 1LL * b) % MOD;
    }

    int addMod(int a, int b, int MOD) {
        return (a + b) % MOD;
    }

    int subMod(int a, int b, int MOD) {
        a -= b;
        if (a < 0)
            a += MOD;
        return a;
    }

    int getValue(char c, std::unordered_map<char, int>& mp) {
        if (mp.find(c) == mp.end()) {
            int sz = mp.size();
            mp.insert({c, sz});
            return sz;
        }
        return mp[c];
    }
    
    std::vector<hashType> hashKmers(minimizer::DnaSequence sequence, int k) {
        std::vector<hashType> ret;
        assert(sequence.getLen() >= k);
        std::unordered_map<char, int> baseValue;
        const int MOD = 1e9 + 7;
        const int BASE = 37;

        hashType lastPower = 1;
        for (int i = 0; i < k - 1; i++)
            lastPower = mulMod(lastPower, BASE, MOD);

        hashType tmpHash = 0;
        for (int i = 0; i < k; i++) {
            tmpHash = mulMod(tmpHash, BASE, MOD);
            tmpHash += getValue(sequence.getCharAt(i), baseValue);
        }
        ret.push_back(tmpHash);

        for (int i = k, n = sequence.getLen(); i < n; i++) {
            tmpHash = subMod(tmpHash, mulMod(getValue(sequence.getCharAt(i - k), baseValue) ,lastPower, MOD), MOD);
            tmpHash = mulMod(tmpHash, BASE, MOD);
            tmpHash += addMod(tmpHash, getValue(sequence.getCharAt(i), baseValue), MOD);
            ret.push_back(tmpHash);
        }
        return ret;
    }


    void push(const minimizer::MinimizerTriple& triple, std::deque<minimizer::MinimizerTriple>& dq) {
        while (!dq.empty() && triple < dq.back()) {
            dq.pop_back();
        }
        dq.push_back(triple);
    }

    void processState(std::deque<minimizer::MinimizerTriple>& dq,
                      std::unordered_set<minimizer::MinimizerTriple>& result) {
        assert(!dq.empty());
        minimizer::MinimizerTriple front = dq.front();
        dq.pop_front();

        result.insert(front);
        while (!dq.empty() && dq.front().h == front.h) {
            front = dq.front();
            dq.pop_front();
            result.insert(front);
        }
        dq.push_front(front);
    }

    void pop(int position, std::deque<minimizer::MinimizerTriple>& dq) {
        while (!dq.empty() && dq.front().position == position)
            dq.pop_front();
    }



} // namespace

namespace minimizer {

    std::unordered_set<MinimizerTriple> computeMinimizers(const std::string &target, int w, int k) {
        std::unordered_set<MinimizerTriple> ret;
        int n = target.size();

        if (n < k) {
            return ret; // ne postoji ni jedan kmer
        }

        if (n < k + w - 1) {//ne postoji ni jedan window od w kmera
            w = n - k + 1; // smanji velicinu trazenog prozora na najvise sta mozes, da se nadje barem jedan minimizer
        }

        DnaSequence sequence = DnaSequence(target.c_str(), n, false);
        DnaSequence rcSequence = sequence.contSubSequence(0, n, true);

        std::vector<hashType> kmerHashes = hashKmers(sequence, k);
        std::vector<hashType> rcKmerHashes = hashKmers(rcSequence, k);
        std::reverse(rcKmerHashes.begin(), rcKmerHashes.end()); // dobimo ih obrnutim redoslijedom

        assert((int) kmerHashes.size() == n - k + 1);
        assert((int) rcKmerHashes.size() == n - k + 1);

        std::deque<MinimizerTriple> dq;

        // queue s maksimumom algoritam
        for (int i = 0; i < w; i++) {
            MinimizerTriple mp1 = MinimizerTriple(kmerHashes[i], i, false);
            MinimizerTriple mp2 = MinimizerTriple(rcKmerHashes[i], i, true);
            push(mp1, dq);
            push(mp2, dq);
        }

        processState(dq, ret);

        for (int i = w; i < kmerHashes.size(); i++) {
            pop(i - w, dq);
            MinimizerTriple mp1 = MinimizerTriple(kmerHashes[i], i, false);
            MinimizerTriple mp2 = MinimizerTriple(rcKmerHashes[i], i, true);
            push(mp1, dq);
            push(mp2, dq);
            processState(dq, ret);
        }

        return ret;
    }




} // namespace minimizer