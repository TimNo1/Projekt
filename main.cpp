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
void getLcskppLengthsFromFastaFile(char* seqfile, vector<pair<pair<string, string>, int>>* lcsks);

int main (int argc, char *argv[])
{
    fprintf(stdout,"Version %d.%d\n",
                Projekt_VERSION_MAJOR,
                Projekt_VERSION_MINOR);
    
    //printHashFromFastaFile(argv[1]);
    //testMatchesFromFastaFile(argv[1]);

    vector<pair<pair<string, string>, int>> lcsks;
    getLcskppLengthsFromFastaFile(argv[1], &lcsks);

    vector<pair<int, pair<string, string>>> lcsksSorted;
    for(const auto& elem:lcsks){
        if(elem.second>0){
            lcsksSorted.push_back(make_pair(elem.second, elem.first));
        }
    }

    sort(lcsksSorted.begin(), lcsksSorted.end());
    for(const auto& elem:lcsksSorted){
        printf("%s %s -> %d\n", elem.second.first.c_str(), elem.second.second.c_str(), elem.first);
    }

    return 0;
}

void getMatches(vector<minimizer::MinimizerTriple> vecA, vector<minimizer::MinimizerTriple> vecB, vector<pair<int,int>>* matches){
    sort(vecA.begin(), vecA.end());
    sort (vecB.begin(), vecB.end());

    int szA = vecA.size();
    int szB = vecB.size();
    // two pointers
    int i = 0;
    int j = 0;
    while (i < szA && j < szB) {
        if (vecA[i].h == vecB[j].h) {
            int oldJ = j;
            while (vecA[i].h == vecB[j].h) {
                matches->push_back({vecA[i].position, vecB[j].position});
                j++;
            }
            i++;
            j = oldJ;
        }
        else if (vecA[i].h > vecB[j].h) {
            j++;
        }
        else {
            i++;
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

    vector<minimizer::MinimizerTriple> vecA = minimizer::computeMinimizers(string(sequenceStringA, sequenceSizeA), W, K);
    vector<minimizer::MinimizerTriple> vecB = minimizer::computeMinimizers(string(sequenceStringB, sequenceSizeB), W, K);

    vector<pair<int, int>> matches;
    getMatches(vecA, vecB, &matches);

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

void getLcskppLengthsFromFastaFile(char* seqfile, vector<pair<pair<string, string>, int>>* lcsks){
    FASTAFILE* ffp = OpenFASTA(seqfile);
    char* sequenceString;
    char* sequenceName;
    int sequenceSize = 0;
    vector<pair<string, vector<minimizer::MinimizerTriple>>> sequences;

    while (ReadFASTA(ffp, &sequenceString, &sequenceName, &sequenceSize)) {
        vector<minimizer::MinimizerTriple> vec = minimizer::computeMinimizers(string(sequenceString, sequenceSize), W, K);
        string name = string(sequenceName);
        sequences.push_back(make_pair(name, vec));

        int size = sequences.size();

        free(sequenceString);
        free(sequenceName);
    }

    CloseFASTA(ffp);

    for (int i = 0; i < sequences.size(); ++i) {
        for (int j = i+1; j < sequences.size(); ++j) {
            vector<pair<int, int>> matches;
            getMatches(sequences[i].second, sequences[j].second, &matches);
            int len = 0;
            vector<pair<int, int>> recon;
            lcskpp_sparse_fast(matches, K, &len, &recon); //K je moguce kriv ovdje
            lcsks->push_back(make_pair(make_pair(sequences[i].first, sequences[j].first), len));
        }
    }


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


void simpleExample(){
    fprintf(stdout, "Enter a string to get all the 3mers of:\n");
    std::string s = "";
    getline(std::cin, s);

    std::vector<finder::kMerTriple*> v = *finder::getKmers(s, 3);

    for (long i=0, n=v.size(); i<n; i++) {
        fprintf(stdout, "%s %ld\n", v[i]->s.c_str(), v[i]->i);
    }
}
