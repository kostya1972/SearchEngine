//
// Created by Kostya on 15.01.2025.
//

#ifndef SEARCHENGINE_SEARCHSERVER_H
#define SEARCHENGINE_SEARCHSERVER_H

#include <vector>
#include <string>
#include "InvertedIndex.h"

class SearchServer {

    std::shared_ptr<InvertedIndex> invertedIndex;
    std::vector<std::vector<std::string>> GetWords(std::vector<std::string>);

public:

    SearchServer() = default;
    SearchServer(std::shared_ptr<InvertedIndex> idx) : invertedIndex(idx) {};
    std::vector<std::vector<std::pair<size_t,float>>> search(const std::vector<std::string> &queries_input, int);

};


#endif //SEARCHENGINE_SEARCHSERVER_H
