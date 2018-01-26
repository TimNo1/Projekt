//
// Created by dario on 12.12.17..
//

#ifndef PROJEKT_LIS_H
#define PROJEKT_LIS_H

#include <minimizer.h>
#include <unordered_map>

template <typename T> class minimizerMap {
    
private:
    std::unordered_map<int, std::pair<long long, int>> positionInArray;
    std::vector<T> array;
    long long currentLength;
public:
    minimizerMap(std::unordered_map<int, std::vector<T>> originalMap){
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
        
        array = std::vector<T>(currentLength);
        
        for(auto& pair: originalMap){
            std::pair<long long, int> positionPair = positionInArray[pair.first];
            for (long long i = positionPair.first; i < positionPair.first + positionPair.second; ++i) {
                array[i] = originalMap[pair.first][i - positionPair.first];
            }
        }
    }
    std::vector<T> get(const int k);
};

namespace lis {

    struct hashTableElement {
        int sequenceIndex, position;
        bool rc;

        //hashTableElement(int _sequenceIndex, int _position, bool _rc)
        //: sequenceIndex(_sequenceIndex), position(_position), rc(_rc) {}
    };
    std::vector<std::pair<int, bool>> getSimilar(int minimizerIndex, std::vector<minimizer::MinimizerTriple> v1,
                                                 minimizerMap<hashTableElement> & ht);
} // namespace lis

#endif //PROJEKT_LIS_H
