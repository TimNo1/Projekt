//
// Created by dario on 12.12.17..
//

#include "lis.h"
#include <algorithm>
#include <iostream>

namespace {
    struct quatr{
        minimizer::MinimizerTriple s;//tu
        int index;
        inline bool operator<(const quatr& other) const {
            return s < other.s;
        }
    };

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

    int lisF(int* v, int n) {
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

    int binarySearch(quatr arr[], int l, int r, int x){
        if (r >= l){
            int mid = l + (r - l)/2;
            if (arr[mid].s.h == x)  //tu
                return arr[mid].index;
            if (arr[mid].s.h > x) //tu
                return binarySearch(arr, l, mid-1, x);
            return binarySearch(arr, mid+1, r, x);
        }
        return -1;
    }
}

namespace lis {
    int getLis(std::vector<minimizer::MinimizerTriple> v1, std::vector<minimizer::MinimizerTriple> v2){//tu
        int nV1,nV2,i,counter=0,r,*p2;
        quatr *p;
        nV1=v1.size();
        nV2=v2.size();
        if(!nV1||!nV2)
            return 0;
        p = (quatr*) malloc (nV1*sizeof(quatr));
        p2 = (int*) malloc (nV2*sizeof(int));// zauzme za svaki, ali kasnije counter kaze kolko ih ima, a mislim da je neptorebno realocirati
        for(i=0;i<nV1;i++){
            p[i].s=v1[i];//možda je bolje s pointerom ovo rijesiti, memorije radi
            p[i].index=i;
        }
        std::sort(p,p+nV1);//sorta po hashevima pa gore ona funkcija treba radit
        for(i=0;i<nV2;i++){
            r=binarySearch(p,0,nV1-1,v2[i].h);
            if(r!=-1){
                p2[counter]=r;
                counter++;
            }
        }
        r=lisF(p2,counter);
        free(p);
        free(p2);
        return r;
    }
}