//
// Created by Donik on 25.1.2018..
//

#ifndef PROJEKT_MINIMIZERMAP_H
#define PROJEKT_MINIMIZERMAP_H


#include <map>
#include "lis.h"

class minimizerMap : public std::unordered_map<int, std::vector<lis::hashTableElement>>{

private:
    std::unordered_map<int, std::pair<long long, int>> positionInArray;
    std::vector<lis::hashTableElement> array;
    long long currentLength;
public:
    minimizerMap(std::unordered_map<int, std::vector<lis::hashTableElement>> originalMap);
    <std::vector<lis::hashTableElement>> operator[](const int k);
};


#endif //PROJEKT_MINIMIZERMAP_H
