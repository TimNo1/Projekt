//
// Created by dario on 04.11.17..
//
#include <assert.h>
#include "DnaSequence.h"

namespace minimizer {
    char DnaSequence::getCharAt(int i) {
        assert(i >= 0 && i < len);

        if (rc)
            return rcMap[s[len - 1 - i]];
        return s[i];
    }

    DnaSequence DnaSequence::contSubSequence(int position, int len, bool rc) {
        assert(position >= 0 && position < this->len);
        assert(position + len <= this->len);
        bool newRc = this->rc;
        if (rc)
            newRc = !newRc;
        return DnaSequence(s + position, len, newRc);
    }
}