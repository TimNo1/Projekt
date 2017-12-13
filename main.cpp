#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <lis.h>
#include "ProjektConfig.h"
#include "finder.h"
#include "lcskpp.h"
#include "minimizer/minimizer.h"

#define K 15
#define W 5

extern "C"{
#include "fasta.h"
}

using namespace std;
void outputInPaf(std::vector<pair<std::string, std::vector<minimizer::MinimizerTriple>>> sequences);
std::vector<pair<std::string, std::vector<minimizer::MinimizerTriple>>> getMinimizersFromFasta(char* f);

int main (int argc, char *argv[])
{
//    fprintf(stdout,"Version %d.%d\n",
//                Projekt_VERSION_MAJOR,
//                Projekt_VERSION_MINOR);
    
    outputInPaf(getMinimizersFromFasta(argv[1]));
    return 0;
}


std::vector<pair<std::string, std::vector<minimizer::MinimizerTriple>>> getMinimizersFromFasta(char* f) {
    FASTAFILE* ffp = OpenFASTA(f);
    char* sequenceString;
    char* sequenceName;
    int sequenceSize = 0;
    vector<pair<string, vector<minimizer::MinimizerTriple>>> sequences;

    while (ReadFASTA(ffp, &sequenceString, &sequenceName, &sequenceSize)) {
        vector<minimizer::MinimizerTriple> vec = minimizer::computeMinimizers(string(sequenceString, sequenceSize), W, K);
        string name = string(sequenceName);
        sequences.push_back(make_pair(name, vec));

        free(sequenceString);
        free(sequenceName);
    }

    CloseFASTA(ffp);
    return sequences;
}


//samo ispise iz pafa ono sto je potrebno da bi jaccard.py radio dobroo
void outputInPaf(std::vector<pair<std::string, std::vector<minimizer::MinimizerTriple>>> sequences) {

    double limit = 0.008;
    for (int i = 0; i < sequences.size(); ++i) {
        for (int j = i+1; j < sequences.size(); ++j) {
            auto l = lis::getLis(sequences[i].second, sequences[j].second);
//            if (l * 1. / std::min(sequences[i].second.size(), sequences[j].second.size()) > limit) {    ...ovako se dobije slican rezultat, mozda se moze bolje s pametnijim limitom
            if (l.first > 9 && l.first > l.second) {
                std::cout << sequences[i].first << "\t";
                for (int z = 0; z < 3; z++)
                    std::cout << "0\t";
                std::cout << "+\t";
                std::cout << sequences[j].first << std::endl;
            }
            else if (l.second > 9) {

                std::cout << sequences[i].first << "\t";
                for (int z = 0; z < 3; z++)
                    std::cout << "0\t";
                std::cout << "-\t";
                std::cout << sequences[j].first << std::endl;
            }
        }
    }
}

void outputSeqPairsByLis(char* seqfile) {
    auto sequences = getMinimizersFromFasta(seqfile);
    std::vector<pair<int, pair<std::string, std::string>>> result;
    for (int i = 0; i < sequences.size(); ++i) {
        for (int j = i+1; j < sequences.size(); ++j) {
            result.push_back({lis::getLis(sequences[i].second, sequences[j].second).first, {sequences[i].first, sequences[j].first}});
        }
    }

    sort(result.begin(), result.end());
    for (pair<int, pair<string, string>> p: result)
        cout << p.second.second << " " << p.second.first << " -> " << p.first << endl;
}


void printHashFromFastaFile(char* seqfile){
    FASTAFILE* ffp = OpenFASTA(seqfile);
    char* sequenceString;
    char* sequenceName;
    int sequenceSize = 0;

    while (ReadFASTA(ffp, &sequenceString, &sequenceName, &sequenceSize)) {
        std::cout<< std::string(sequenceName)<<std::endl;
        std::cout<< std::string(sequenceString, sequenceSize)<<std::endl;
        
        std::vector<minimizer::MinimizerTriple> vec = minimizer::computeMinimizers(std::string(sequenceString, sequenceSize), 20, 15);
        
        for (const auto& it: vec) {
            std::cout << "POSITION: " << it.position << "   HASH: " << it.h << "    IS_RC: " << it.rc << std::endl;
        }
        
        std::cout<<std::endl;
        
        free(sequenceString);
        free(sequenceName);
    }
    
    CloseFASTA(ffp);
}

