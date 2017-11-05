//
// Created by dario on 04.11.17..
//

#ifndef PROJEKT_DNASEQUENCE_H
#define PROJEKT_DNASEQUENCE_H

#include <map>

namespace minimizer {
    class DnaSequence {
    private:
        const char* s;
        int len;
        bool rc;
        std::map<char, char> rcMap = {{'A', 'T'}, {'C', 'G'}, {'T', 'A'}, {'G', 'C'}};
    public:
        DnaSequence(const char* _s, int _len, bool _rc)
                : s(_s), len(_len), rc(_rc) {}

        inline bool isRc() {
            return rc;
        }

        inline int getLen() {
            return len;
        }

        char getCharAt(int i);

        DnaSequence contSubSequence(int position, int len, bool rc);

    };
} // namespace minimizer

#endif //PROJEKT_DNASEQUENCE_H
