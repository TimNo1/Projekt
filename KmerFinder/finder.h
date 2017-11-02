#ifndef FINDER_H
#define FINDER_H

#include<string>
#include<vector>

namespace finder
{
    struct kMerTriple {
        std::string s;
        long i;
        bool p;
    };
    
    std::vector<kMerTriple*>* getKmers(std::string s, int k);
} // namespace finder


#endif
