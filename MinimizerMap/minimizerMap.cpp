//
// Created by Donik on 25.1.2018..
//

#include "minimizerMap.h"

minimizerMap::minimizerMap(std::unordered_map<int, std::vector<lis::hashTableElement>> originalMap) {
    long long size = 0;
    positionInArray = std::unordered_map(originalMap.size());
    std::map<int, std::vector<int>> lengths = std::map<int, std::vector<int>>();

    int i=0;
    for (auto& pair : originalMap) {
        lengths[pair.second.size()].emplace_back(pair.first);
    }

    for(auto& pair : lengths){
        for(auto& hash : pair.second){
            positionInArray.emplace(hash, std::pair(currentLength, pair.first));
            currentLength+=pair.first;
        }
    }

    array = std::vector<lis::hashTableElement>(size);

    for(auto& pair: originalMap){
        std::pair<long long, int> positionPair = positionInArray[pair.first];
        for (long long i = positionPair.first; i < positionPair.second; ++i) {
            array[i] = originalMap[pair.first][i];
        }
    }
}

std::vector<lis::hashTableElement> minimizerMap::operator[](const int k){
    if (minimizerMap::positionInArray.count(k)>0){
        std::pair pair = positionInArray[k];
        return  std::vector<lis::hashTableElement>(minimizerMap::array.begin()+pair.first, minimizerMap::array.begin() + pair.first + pair.second);
    }

    return array;
}

