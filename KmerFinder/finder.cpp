#include "finder.h"

namespace finder
{
    std::vector<kMerTriple*>* getKmers(std::string s, int k){
        if (s.length()<k) {
            return new std::vector<kMerTriple*>(0);
        }
        
        std::vector<kMerTriple*>* kmers = new std::vector<kMerTriple*>(s.length()-k+1);
        
        for (long i=0, n=s.length(); i<n-k+1; i++) {
            kMerTriple* triple = new kMerTriple();
            triple->i = i;
            triple->s = s.substr(i, k);
            triple->p = 0;
            
            (*kmers)[i] = triple;
        }
        
        return kmers;
    }
} // namespace finder
