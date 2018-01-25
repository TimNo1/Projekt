//
// Created by Donik on 25.1.2018..
//

#include "minimizerMap.h"

minimizerMap::minimizerMap(std::unordered_map<int, std::vector<lis::hashTableElement>> originalMap) {
    currentLength = 0;
    positionInArray = std::unordered_map<int, std::pair<long long, int>>(originalMap.size());
    std::map<int, std::vector<int>> lengths = std::map<int, std::vector<int>>();

    for (auto& pair : originalMap) {
        lengths[pair.second.size()].emplace_back(pair.first);
    }

    for(auto& pair : lengths){
        for(auto& hash : pair.second){
            positionInArray.emplace(hash, std::pair<long long, int>(currentLength, pair.first));
            currentLength+=pair.first;
        }
    }

    array = std::vector<lis::hashTableElement>(currentLength);

    for(auto& pair: originalMap){
        std::pair<long long, int> positionPair = positionInArray[pair.first];
        for (long long i = positionPair.first; i < positionPair.first + positionPair.second; ++i) {
            array[i] = originalMap[pair.first][i - positionPair.second];
        }
    }
}

std::vector<lis::hashTableElement> minimizerMap::get(const int k){
    if (minimizerMap::positionInArray.count(k)>0){
        std::pair<long long, int> pair = positionInArray[k];
        return  std::vector<lis::hashTableElement>(minimizerMap::array.begin()+pair.first, minimizerMap::array.begin() + pair.first + pair.second);
    }

    return array;
}

