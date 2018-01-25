//
// Created by Donik on 25.1.2018..
//

#include "minimizerMap.h"

minimizerMap::minimizerMap(std::unordered_map<int, std::vector<lis::hashTableElement>> originalMap) {
    long long size = 0;
    positionInArray = std::unordered_map(originalMap.size());

    for (auto& pair : originalMap) {
        currentLength+=pair.second.size();
        positionInArray.emplace(pair.first, std::pair(currentLength, pair.second.size()));
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

