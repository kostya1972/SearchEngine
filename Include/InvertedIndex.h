#ifndef INVERTED_INDEX
#define INVERTED_INDEX


#include <vector>
#include <string>
#include <map>
#include <future>
#include <chrono>
#include <thread>
#include "ConverterJSON.h"

struct Entry {
    size_t docId, count;

    bool operator==(const Entry &other) const {
        return (docId == other.docId && count == other.count);
    }
};

class InvertedIndex {

    std::shared_ptr<ConverterJSON> converterJson;
    std::mutex mutex;
    std::map<std::string, std::vector<Entry>> freqDictionary{};

public:

    InvertedIndex() = default;
    InvertedIndex(InvertedIndex &);
    InvertedIndex(std::shared_ptr<ConverterJSON> cJson) : converterJson(cJson){};
    InvertedIndex &operator=(const InvertedIndex &);
    std::vector<Entry> GetWordCount(const std::string &);
    void UpdateDocumentBase(const std::vector<std::string>, size_t);
    void UpdateLoop(bool &);
};

#endif