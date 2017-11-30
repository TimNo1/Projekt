#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include "ProjektConfig.h"
#include "finder.h"
#include "lcskpp.h"
#include "minimizer/minimizer.h"

#define K 15
#define W 20

extern "C"{
#include "fasta.h"
}

using namespace std;

void printHashFromFastaFile(char* seqfile);
void testMatchesFromFastaFile(char* seqfile);
void simpleExample();

int main (int argc, char *argv[])
{
    fprintf(stdout,"Version %d.%d\n",
                Projekt_VERSION_MAJOR,
                Projekt_VERSION_MINOR);
    
    //printHashFromFastaFile(argv[1]);
    testMatchesFromFastaFile(argv[1]);
    
    return 0;
}

void getMatches(const string& a, const string& b, vector<pair<int,int>>* matches){
    unordered_set<minimizer::MinimizerTriple> vecA = minimizer::computeMinimizers(a, W, K);
    unordered_set<minimizer::MinimizerTriple> vecB = minimizer::computeMinimizers(b, W, K);


    for(const auto& minimizerA : vecA){
        for(const auto& minimizerB : vecB){
            if (minimizerA.h == minimizerB.h){
                matches->push_back(make_pair(minimizerA.position, minimizerB.position));
            }
        }
    }

    sort(matches->begin(), matches->end());
}

void testMatchesFromFastaFile(char* seqfile){
    FASTAFILE* ffp = OpenFASTA(seqfile);
    char* sequenceStringA;
    char* sequenceNameA;
    int sequenceSizeA = 0;
    char* sequenceStringB;
    char* sequenceNameB;
    int sequenceSizeB = 0;

    ReadFASTA(ffp, &sequenceStringA, &sequenceNameA, &sequenceSizeA);
    ReadFASTA(ffp, &sequenceStringB, &sequenceNameB, &sequenceSizeB);

    vector<pair<int, int>> matches;
    getMatches(string(sequenceStringA, sequenceSizeA), string(sequenceStringB, sequenceSizeB), &matches);

    int len = 0;
    vector<pair<int, int>> recon;
    lcskpp_sparse_fast(matches, K, &len, &recon); //K je moguce kriv ovdje

    for (const pair<int, int> pair : recon){
        printf("(%d, %d)\n", pair.first, pair.second);
    }
    printf("%d", len);

    free(sequenceStringA);
    free(sequenceNameA);
    free(sequenceStringB);
    free(sequenceNameB);

    CloseFASTA(ffp);
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
