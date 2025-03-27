#include "SearchServer.h"

std::vector<std::vector<std::pair<size_t,float>>> SearchServer::search(const std::vector<std::string> &queriesInput, int maxResponses)
{
    std::vector<std::vector<std::pair<size_t,float>>> relevantAnswers{};
    std::vector<std::vector<std::string>> searchWords(GetWords(queriesInput)); //в каждом векторе запрос, разбитый на отдельные слова

    for (auto &request : searchWords) //перебираем все запросы
    {
        std::map<size_t,size_t> tempMap{};//для временного хранения пар(docId,count)
        std::multimap<size_t,size_t> swapMap{};//для временного хранения перевернутых пар(count,docId)

        for (auto &word : request) //отдельные слова запроса
        {
            const std::vector<Entry> wordCount = invertedIndex->GetWordCount(word);

            for (auto &entry : wordCount) //документы с данным словом запроса
            {
                auto it = tempMap.find(entry.docId);
                if (it == tempMap.end()) //если документа в map нет, вставляем
                    tempMap.insert(std::make_pair(entry.docId, entry.count));
                else
                   it->second = it->second + entry.count; //если есть складываем count документов, и
            }                                             // находим абсолютную релевантность запроса для данного документа
        }
        std::vector<std::pair<size_t,float>> tempRelative{};

        if (!tempMap.empty()) //если ответ есть обрабатываем его
        {
            for (auto temp : tempMap) //меняем значения first(docId) и second(count) местами, и находим максимальную абсолютную релевантность,
            {                         //которая будет находится в конце swapMap
                swapMap.insert(std::make_pair(temp.second, temp.first));
            }

            float maxAbsolut;
            maxAbsolut = float(swapMap.rbegin()->first); //максимальная абсолютная релевантность

            int i = 0;
            //auto maxResponses =  invertedIndex->GetConverterJson()->GetSearchConfig().maxResponses;//для ограничения кол_ва ответов

            auto it = swapMap.rbegin();
            for (it; it != swapMap.rend(); it++)//заполняем вектор на каждый запрос
            {
                if (++i > maxResponses)// ограничение количества ответов
                    break;
                auto absolut = float(it->first);
                float real = absolut / maxAbsolut * 1000000; //форматируем реальное значение float для вывода в json файл
                tempRelative.emplace_back(it->second, truncf(real) / 1000000);
            }
        }
        relevantAnswers.push_back(tempRelative);// если ответа нет помещаем в relevantAnswers пустой вектор
    }                                           // если ответ есть , заполненый

    return relevantAnswers;
}
// разбиваем запросы на отдельные слова
std::vector<std::vector<std::string>> SearchServer::GetWords(std::vector<std::string> requests)
{
    std::vector<std::vector<std::string>> searchWords;

    for (int i = 0; i < requests.size(); i++)
    {
        std::vector<std::string> tempWords;
        int point = 0;//указатель на начало слова

        for (int j = 0; j < requests[i].length() + 1; j++)
        {   //вытаскиваем слово из строки
            if (isspace(requests[i][j]) || requests[i][j] == '\0') {
                if (j != 0) {   //если пробел не в начале строки
                    std::string tempStr = requests[i].substr(point, j - point);
                    tempWords.push_back(tempStr);
                }//выкидываем пробелы
                while (isspace(requests[i][j]))
                    j++;
                point = j; //ставим указатель на начало следующего слова
            }
        }
        searchWords.push_back(tempWords);
    }
    return searchWords;
}
