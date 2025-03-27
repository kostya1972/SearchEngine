//
// Created by Kostya on 14.01.2025.
//

#ifndef SEARCHENGINE_CONVERTERJSON_H
#define SEARCHENGINE_CONVERTERJSON_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "nlohmann/json.hpp"

struct SearchConfig {
    std::string name;
    std::string version;
    int maxResponses;
    time_t updateTime;
    std::vector<std::string> files;
};

class ConverterJSON {

    SearchConfig searchConfig;

public:

    ConverterJSON();
    ConverterJSON(ConverterJSON *){};
    std::vector<std::string> GetTextDocument(const size_t);
    std::vector<std::string> GetRequests();
    const SearchConfig &GetSearchConfig() const;
    void PutAnswers(std::vector<std::vector<std::pair<size_t , float>>> answers);

};


#endif //SEARCHENGINE_CONVERTERJSON_H
