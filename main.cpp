#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include "lis.h"
#include <unordered_map>
#include "ProjektConfig.h"
#include "finder.h"
#include "lcskpp.h"

#include "minimizer/minimizer.h"
#include "thread_pool/thread_pool.hpp"


#define K 15
#define W 5

extern "C"{
#include "fasta.h"
}

using namespace std;
void outputOverlaps(const std::vector<pair<std::string, std::vector<minimizer::MinimizerTriple>>>& sequences,
                    std::unordered_map<int, std::vector<lis::hashTableElement>>& ht);
std::vector<pair<std::string, std::vector<minimizer::MinimizerTriple>>> getMinimizersFromFasta(char* f);
std::unordered_map<int, std::vector<lis::hashTableElement>> generateHashTable(const std::vector<pair<std::string, std::vector<minimizer::MinimizerTriple>>>& mins);

int main (int argc, char *argv[])
{
//    fprintf(stdout,"Version %d.%d\n",
//                Projekt_VERSION_MAJOR,
//                Projekt_VERSION_MINOR);
    
    
    auto mins = getMinimizersFromFasta(argv[1]);
    auto ht = generateHashTable(mins);
    outputOverlaps(mins, ht);
    return 0;
}

std::unordered_map<int, std::vector<lis::hashTableElement>> generateHashTable(const std::vector<pair<std::string, std::vector<minimizer::MinimizerTriple>>>& mins) {
    std::unordered_map<int, std::vector<lis::hashTableElement>> ret;
//    for (int i = 0; i < (int)mins.size(); i++) {
//        for (int j = 0; j < (int)mins[i].second.size(); j++) {
//            minimizer::MinimizerTriple mini = mins[i].second[j];
//            ret[mini.h].push_back(lis::hashTableElement(i, j, mini.rc));
//        }
//    }
    return ret;

}

void insertInTable(std::unordered_map<int, std::vector<lis::hashTableElement>>& ht, int sequenceIndex, std::vector<minimizer::MinimizerTriple> mins) {
    for (int i = 0, n = mins.size(); i < n; i++) {
        ht[mins[i].h].push_back(lis::hashTableElement(sequenceIndex, i, mins[i].rc));
    }
}

pair<string, vector<minimizer::MinimizerTriple>> returnNameMinimizerPair(const std::string name, std::string target, int w, int k) {
    vector<minimizer::MinimizerTriple> vec = minimizer::computeMinimizers(target, W, K);
    return {name, vec};
}

std::vector<pair<std::string, std::vector<minimizer::MinimizerTriple>>> getMinimizersFromFasta(char* f) {
    FASTAFILE* ffp = OpenFASTA(f);
    char* sequenceString;
    char* sequenceName;
    int sequenceSize = 0;
    std::shared_ptr<thread_pool::ThreadPool> thread_pool =
    thread_pool::createThreadPool();
    
    vector<future<pair<string, vector<minimizer::MinimizerTriple>>>> future_sequnce;
    
    while (ReadFASTA(ffp, &sequenceString, &sequenceName, &sequenceSize)) {
        string sequence = string(sequenceString, sequenceSize);
        string name = string(sequenceName);
        
        free(sequenceString);
        free(sequenceName);
        
        future_sequnce.emplace_back(thread_pool->submit_task(returnNameMinimizerPair, name, sequence, W, K));
    }
    
    vector<pair<string, vector<minimizer::MinimizerTriple>>> sequences = *new vector<pair<string, vector<minimizer::MinimizerTriple>>>(future_sequnce.size());
    
    int i=0;
    for (auto& it: future_sequnce) {
        it.wait();
        if(it.valid()) {
            pair<string, vector<minimizer::MinimizerTriple>> pair = it.get();
            cout<<pair.first<<endl;
            try {
                sequences[i] = pair;
                i++;
            }  catch (const std::length_error& le) {
                std::cerr << "Length error: " << le.what() << '\n';
            }
        }
    }

    CloseFASTA(ffp);
    return sequences;
}

void outputInPaf(const string& name1, const string& name2, bool plusStrand) {
    printf("%s\t0\t0\t0\t", name1.c_str());
    if (plusStrand)
        printf("+");
    else
        printf("-");
    printf("\t%s\n", name2.c_str());
}

std::pair<int, std::vector<std::pair<int, bool>>> getSimilarWrapper(int sequenceIndex, std::vector<minimizer::MinimizerTriple> v1,
                                                                    std::unordered_map<int, std::vector<lis::hashTableElement>>& ht){
    auto lis = lis::getSimilar(sequenceIndex, v1, ht);
    
    return {sequenceIndex, lis};
}
//samo ispise iz pafa ono sto je potrebno da bi jaccard.py radio dobroo
void outputOverlaps(const std::vector<pair<std::string, std::vector<minimizer::MinimizerTriple>>>& sequences,
                    std::unordered_map<int, std::vector<lis::hashTableElement>>& ht) {

    std::shared_ptr<thread_pool::ThreadPool> thread_pool =
    thread_pool::createThreadPool();
    std::vector<std::future<std::pair<int, std::vector<std::pair<int, bool>>>>> thread_futures;
    
    //double limit = 0.008;
    for (int i = 0; i < sequences.size(); ++i) {
        thread_futures.emplace_back(thread_pool->submit_task(getSimilarWrapper, i, sequences[i].second, std::ref(ht)));
    }
    
    for (auto& it: thread_futures) {
        it.wait();
        std::pair<int, std::vector<std::pair<int, bool>>> similar = it.get();
        insertInTable(ht, similar.first, sequences[similar.first].second);
        for (auto element: similar.second) {
            outputInPaf(sequences[it.get().first].first, sequences[element.first].first, element.second);
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

