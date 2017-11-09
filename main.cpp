#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unordered_set>
#include "ProjektConfig.h"
#include "finder.h"
#include "minimizer/minimizer.h"

extern "C"{
#include "fasta.h"
}
void printHashFromFastaFile(char* seqfile);
void simpleExample();

int main (int argc, char *argv[])
{
    fprintf(stdout,"Version %d.%d\n",
                Projekt_VERSION_MAJOR,
                Projekt_VERSION_MINOR);
    
    printHashFromFastaFile(argv[1]);
    
    return 0;
}

void printHashFromFastaFile(char* seqfile){
    FASTAFILE* ffp = OpenFASTA(seqfile);
    char* sequenceString;
    char* sequenceName;
    int sequenceSize = 0;
    
    while (ReadFASTA(ffp, &sequenceString, &sequenceName, &sequenceSize)) {
        std::cout<< std::string(sequenceName)<<std::endl;
        std::cout<< std::string(sequenceString, sequenceSize)<<std::endl;
        
        std::unordered_set<minimizer::MinimizerTriple> vec = minimizer::computeMinimizers(std::string(sequenceString, sequenceSize), 20, 15);
        
        for (const auto& it: vec) {
            std::cout << "POSITION: " << it.position << "   HASH: " << it.h << "    IS_RC: " << it.rc << std::endl;
        }
        
        std::cout<<std::endl;
        
        free(sequenceString);
        free(sequenceName);
    }
    
    CloseFASTA(ffp);
}


void simpleExample(){
    fprintf(stdout, "Enter a string to get all the 3mers of:\n");
    std::string s = "";
    getline(std::cin, s);

    std::vector<finder::kMerTriple*> v = *finder::getKmers(s, 3);

    for (long i=0, n=v.size(); i<n; i++) {
        fprintf(stdout, "%s %ld\n", v[i]->s.c_str(), v[i]->i);
    }
}
