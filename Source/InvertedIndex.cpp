#include "../Include/InvertedIndex.h"
#include "../Include/SearchServer.h"

InvertedIndex::InvertedIndex(InvertedIndex &invertedIndex)
{
    freqDictionary = invertedIndex.freqDictionary;
}

InvertedIndex &InvertedIndex::operator=(const InvertedIndex &invertedIndex)
{
    if (this == &invertedIndex)
        return *this;
    freqDictionary = invertedIndex.freqDictionary;

    return *this;
}
// doc - текст разбитый на слова, id - номер документа
void InvertedIndex::UpdateDocumentBase(const std::vector<std::string> doc, size_t id)
{
        for (size_t i = 0; i < doc.size(); i++)
        {
            std::map<std::string,std::vector<Entry>>::iterator it;
            it = freqDictionary.find(doc[i]);

            if (it == freqDictionary.end())//если слова нет в словаре, вставляем пару: слово и Entry(idDoc,count)
            {
                size_t count = 1;
                Entry tempEntry{id, count};
                std::vector<Entry> tempVector{tempEntry};

                std::lock_guard<std::mutex> lockGuard(mutex);
                freqDictionary.insert(std::make_pair(doc[i], tempVector));
            }
            else if (it != freqDictionary.end())//если слово есть в словаре
            {
                std::vector<Entry>::iterator it2 = it->second.begin();

                for (it2; it2 != it->second.end(); it2++)
                {
                    if (it2->docId == id)
                    { //если слово есть в текущем документе, увеличиваем count
                        std::lock_guard<std::mutex> lockGuard(mutex);
                        it2->count++;
                        break;
                    }
                }
                if (it2 == it->second.end())
                { //если слова нет в текущем документе, вставляем: Entry{id, count}
                    size_t count = 1;
                    Entry tempEntry{id, count};
                    std::lock_guard<std::mutex> lockGuard(mutex);
                    it->second.push_back(tempEntry);
                }
            }
        }
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word)
{
    auto it = freqDictionary.begin();
    std::vector<Entry> vec;

    for (it; it != freqDictionary.end(); it++)
    {
        if (it->first == word)
        {
            vec = it->second;
            break;
        }
    }
    return vec;
}

void InvertedIndex::UpdateLoop(bool &interrupt)
{
    std::vector<std::string> text;
    time_t  updateTime = converterJson->GetSearchConfig().updateTime;
    time_t currentTime;
    time_t triggerTime;
    time_t  oldTime = std::time(nullptr);
    auto filesCount = converterJson->GetSearchConfig().files.size();
    unsigned int processors = std::thread::hardware_concurrency();

    if (processors < 4) // если процессоров меньше 4 или ошибка функции
        processors = 4; // все равно запускаем два потока(два уже запущены)

    while (true) // цикл индексации дкументов
    {
        if (interrupt) //прерываем цикл из main и завершаем поток
            return;
        currentTime = std::time(nullptr);
        triggerTime = currentTime - oldTime;

        if (triggerTime == updateTime || triggerTime == 0)
        {
            for (size_t i = 0; i < filesCount; i++) // цикл длиной в кол-во документов
            {
                std::vector<std::future<void>> future;

                for (size_t j = 0; j < (processors - 2); j++) // запуск потоков по количеству процессоров
                {                                             // два потока уже запущены
                    if (i >= filesCount)
                    { //если кол-во процессоров не четно кол-ву документов
                        i++;
                        break;
                    }
                    text = converterJson->GetTextDocument(i);
                    future.push_back(std::async(std::launch::async, &InvertedIndex::UpdateDocumentBase, this,text,i));
                    i++;
                }
                i--;
                for (size_t j = 0; j < future.size(); j++)
                    future[j].wait();
            }
            if (currentTime != 0)
                oldTime = currentTime;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
