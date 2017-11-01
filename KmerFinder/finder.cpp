#include "finder.h"

namespace std
{
    vector<kMerTriple*>* getKmers(string s, int k){
        if (s.length()<k) {
            return new vector<kMerTriple*>(0);
        }
        
        vector<kMerTriple*>* kmers = new vector<kMerTriple*>(s.length()-k);
        
        for (long i=0, n=s.length(); i<n-k; i++) {
            
            kMerTriple* triple = new kMerTriple();
            triple->i = i;
            triple->s = s.substr(i, k);
            triple->p = 0;
            
            (*kmers)[i] = triple;
        }
        
        return kmers;
    }
}
