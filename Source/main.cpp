#include <iostream>
#include <memory>
#include "ConverterJSON.h"
#include "InvertedIndex.h"
#include "SearchServer.h"
//#include "tests.h"

int main() {

    //testing::InitGoogleTest();
    //return RUN_ALL_TESTS();

    auto converterJson = std::make_shared<ConverterJSON>();
    auto invertedIndex = std::make_shared<InvertedIndex>(converterJson);

    bool interrupt = false;

    std::cout << "Starting.\n" << converterJson->GetSearchConfig().name << " " << converterJson->GetSearchConfig().version << std::endl;
    // запускаем поток с циклом индексации файлов
    auto future = std::async(std::launch::async, &InvertedIndex::UpdateLoop,invertedIndex,std::ref(interrupt));
    std::this_thread::sleep_for(std::chrono::seconds(1));

    while (true)
    {
        std::string str;
        std::cout << "Enter [request] for request, or [quit]: ";
        std::cin >> str;

        if (str == "quit")
        {
            interrupt = true; //посылаем сигнал прерывания циклу индексации файлов в дочернем потоке
            std::this_thread::sleep_for(std::chrono::seconds(1));
            exit(0);
        }
        else if (str == "request")
        {
            auto  searchServer = std::make_shared<SearchServer>(invertedIndex);
            std::vector<std::string> text = converterJson->GetRequests();
            int maxResponses = converterJson->GetSearchConfig().maxResponses;
            std::vector<std::vector<std::pair<size_t,float>>> answers = searchServer->search(text, maxResponses);
            converterJson->PutAnswers(answers);
        }
        else
        {
            std::cout << "Wrong input." << std::endl;
        }
    }
}
