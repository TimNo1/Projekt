#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unordered_set>
#include "ProjektConfig.h"
#include "finder.h"
#include "minimizer/minimizer.h"
#include "fasta.h"

int main (int argc, char *argv[])
{
    fprintf(stdout,"Version %d.%d\n",
                Projekt_VERSION_MAJOR,
                Projekt_VERSION_MINOR);
    
    fprintf(stdout, "Enter a string to get all the 3mers of:\n");
    std::string s = "";
    getline(std::cin, s);

    std::vector<finder::kMerTriple*> v = *finder::getKmers(s, 3);

    for (long i=0, n=v.size(); i<n; i++) {
        fprintf(stdout, "%s %ld\n", v[i]->s.c_str(), v[i]->i);
    }

    std::cout << std::endl;
    std::cout << "minimizer example" << std::endl;
    std::unordered_set<minimizer::MinimizerTriple, minimizer::MinimizerTripleHasher> vec = minimizer::computeMinimizers("TAGCATGCGTAAGCG", 4, 3);
    for (const auto& it: vec) {
        std::cout << "POSITION: " << it.position << "   HASH: " << it.h << "    IS_RC: " << it.rc << std::endl;
    }
    return 0;
}

