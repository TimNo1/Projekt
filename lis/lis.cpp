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

    int lisF(int* v, int n, int progress) {
        if (n == 0)
            return 0;

        std::vector<int> tail(n, 0);
        int length = 1; // always points empty slot in tail

        tail[0] = v[0];
        for (int i = progress; i != n * progress; i += progress) {
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

    int lds(int* v, int n) {
        return lisF(v + n - 1, n, -1);
    }

    int lisF(int* v, int n) {
        return lisF(v, n, 1);
    }

}

namespace lis {
    std::pair<int, int> getLis(std::vector<minimizer::MinimizerTriple> v1, std::vector<minimizer::MinimizerTriple> v2){//tu
        int nV1,nV2,i,counter=0, counterD=0, *p2, *p2D;
        nV1=v1.size();
        nV2=v2.size();
        if(!nV1||!nV2)
            return {0, false};
        p2 = (int*) malloc (nV2*sizeof(int));// zauzme za svaki, ali kasnije counter kaze kolko ih ima, a mislim da je neptorebno realocirati
        p2D = (int*) malloc(nV2*sizeof(int));
        std::unordered_map<minimizer::hashType, int> positions;
        std::unordered_map<minimizer::hashType, int> rcPositions;
        for(i=0;i<nV1;i++){
            if (v1[i].rc)
                rcPositions.insert({v1[i].h, i});
            else
                positions.insert({v1[i].h, i});
        }
        for(i=0;i<nV2;i++){
            int r = -1;
            int rrc = -1;
            if (v2[i].rc) {
                if (rcPositions.find(v2[i].h) != rcPositions.end())
                    r = rcPositions[v2[i].h];

                if (positions.find(v2[i].h) != positions.end())
                    rrc = positions[v2[i].h];
            }
            else {
                if (positions.find(v2[i].h) != positions.end())
                    r = positions[v2[i].h];

                if (rcPositions.find(v2[i].h) != rcPositions.end())
                    rrc = rcPositions[v2[i].h];
            }

            if(r!=-1){
                p2[counter]=r;
                counter++;
            }

            if (rrc!=-1) {
                p2D[counterD]=rrc;
                counterD++;
            }
        }
        int ret=lisF(p2,counter);
        int ret2 = lds(p2D, counterD);
        free(p2);
        free(p2D);
        return {ret, ret2};
    }
}
