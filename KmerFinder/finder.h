#ifndef FINDER_H
#define FINDER_H

#include<string>
#include<vector>

namespace std
{
    struct kMerTriple {
        string s;
        long i;
        bool p;
    };
    
    vector<kMerTriple*>* getKmers(string s, int k);
}


#endif
