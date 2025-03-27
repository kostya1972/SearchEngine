//
// Created by Kostya on 11.03.2025.
//

#ifndef SEARCHENGINE_TESTS_H
#define SEARCHENGINE_TESTS_H

#include "gtest/gtest.h"

/////////////////////////////////////////////////////////////////////////
using namespace std;
void TestInvertedIndexFunctionality(
        const vector<vector<string>>& docs,
        const vector<string>& requests,
        const vector<vector<Entry>>& expected
) {
    vector<vector<Entry>> result;
    InvertedIndex *invertedIndex = new InvertedIndex();

    for (size_t i = 0; i < docs.size(); i++)
    {
        invertedIndex->UpdateDocumentBase(docs[i],i);
    }
    for(auto& request : requests) {
        vector<Entry> word_count = invertedIndex->GetWordCount(request);
        result.push_back(word_count);
    }
    ASSERT_EQ(result, expected);
}
TEST(TestCaseInvertedIndex, TestBasic) {
const vector<vector<string>> docs = {
        {"london", "is", "the", "capital", "of", "great", "britain"},
        {"big", "ben", "is", "the", "nickname", "for", "the", "Great", "bell", "of", "the", "striking", "clock"}
};
const vector<string> requests = {"london", "the"};
const vector<vector<Entry>> expected = {
        {
                {0, 1}
        },
        {
                {0, 1}, {1, 3}
        }
};
TestInvertedIndexFunctionality(docs, requests, expected);
}
TEST(TestCaseInvertedIndex, TestBasic2) {
const vector<vector<string>> docs = {
        {"milk", "milk", "milk", "milk", "water", "water", "water"},
        {"milk", "water", "water"},
        {"milk", "milk", "milk", "milk", "milk", "water", "water", "water", "water", "water"},
         {"americano", "cappuccino"}
};
const vector<string> requests = {"milk", "water", "cappuccino"};
const vector<vector<Entry>> expected = {
        {
                {0, 4}, {1, 1}, {2, 5}
        }, {
                {0, 3}, {1, 2}, {2, 5}
        }, {
                {3, 1}
        }
};
TestInvertedIndexFunctionality(docs, requests, expected);
}
TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord) {
const vector<vector<string>> docs = {
        {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l"},
        {"statement"}
};
const vector<string> requests = {"m", "statement"};
const vector<vector<Entry>> expected = {
        {
        }, {
                {1, 1}
        }
};
TestInvertedIndexFunctionality(docs, requests, expected);
}
////////////////////////////////////////////////////////////////////////////
TEST(TestCaseSearchServer, TestSimple)
{
    const vector<vector<string>> docs = {
            {"milk", "milk", "milk", "milk", "water", "water", "water"},
            {"milk", "water", "water"},
            {"milk", "milk", "milk", "milk", "milk", "water", "water", "water", "water", "water"},
            {"americano", "cappuccino"}
    };
    const vector<string> request = {"milk water sugar"};
    const vector<vector<pair<size_t,float>>> expected = {
            {
                    {2, 1},
                    {0, 0.7},
                    {1, 0.3}
            }
    };
    auto converterJson = std::make_shared<ConverterJSON>();
    auto invertedIndex = std::make_shared<InvertedIndex>(converterJson);
    for (size_t i = 0; i < docs.size(); i++)
    {
        invertedIndex->UpdateDocumentBase(docs[i], i);
    }
    auto searchServer = std::make_shared<SearchServer>(invertedIndex);
    vector<vector<pair<size_t,float>>> result = searchServer->search(request, converterJson->GetSearchConfig().maxResponses);

    ASSERT_EQ(result, expected);
}
TEST(TestCaseSearchServer, TestTop5)
{
    const vector<vector<string>> docs = {
            {"london", "is", "the", "capital", "of", "great", "britain"},
            {"paris", "is", "the", "capital", "of", "france"},
            {"berlin", "is", "the", "capital", "of", "germany"},
            {"rome", "is", "the", "capital", "of", "italy"},
            {"madrid", "is", "the", "capital", "of", "spain"},
            {"lisboa", "is", "the", "capital", "of", "portugal"},
            {"bern", "is", "the", "capital", "of", "switzerland"},
            {"moscow", "is", "the", "capital", "of", "russia"},
            {"kiev", "is", "the", "capital", "of", "ukraine"},
            {"minsk", "is", "the", "capital", "of", "belarus"},
            {"astana", "is", "the", "capital", "of", "kazakhstan"},
            {"beijing", "is", "the", "capital", "of", "china"},
            {"tokyo", "is", "the", "capital", "of", "japan"},
            {"bangkok", "is", "the", "capital", "of", "thailand"},
            {"welcome", "to", "moscow", "the", "capital", "of", "russia", "the", "third", "rome"},
            {"amsterdam", "is", "the", "capital", "of", "netherlands"},
            {"helsinki", "is", "the", "capital", "of", "finland"},
            {"oslo", "is", "the", "capital", "of", "norway"},
            {"stockholm", "is", "the", "capital", "of", "sweden"},
            {"riga", "is", "the", "capital", "of", "latvia"},
            {"tallinn", "is", "the", "capital", "of", "estonia"},
            {"warsaw", "is", "the", "capital", "of", "poland"}
    };
    const vector<string> request = {"moscow is the capital of russia"};
    const vector<vector<pair<size_t,float>>> expected = {
            {
                    {14, 1},
                    {7, 1},
                    {21, 0.666666},
                    {20, 0.666666},
                    {19, 0.666666}
            }
    };
    auto converterJson = std::make_shared<ConverterJSON>();
    auto invertedIndex = std::make_shared<InvertedIndex>(converterJson);
    for (size_t i = 0; i < docs.size(); i++)
    {
        invertedIndex->UpdateDocumentBase(docs[i], i);
    }
    auto searchServer = std::make_shared<SearchServer>(invertedIndex);
    vector<vector<pair<size_t,float>>> result = searchServer->search(request, converterJson->GetSearchConfig().maxResponses);

    ASSERT_EQ(result, expected);
}

#endif //SEARCHENGINE_TESTS_H
