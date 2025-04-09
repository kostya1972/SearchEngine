#include "ConverterJSON.h"
#include "EngineExceptions.h"

ConverterJSON::ConverterJSON()
{
    std::ifstream file;
    nlohmann::json configFile;
    try
    {
        file.open(".\\Json_files\\config.json");
        if (!file.is_open())
        {
            throw EngineException(CONF_FILE_MISSING);
        }

        file >> configFile;
        nlohmann::json::iterator it = configFile.find("config");

        if (it == configFile.end())
        {
            file.close();
            throw EngineException(CONF_FILE_EMPTY);
        }
    }
    catch (EngineException &e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
        exit(1);
    }
    searchConfig.name = configFile["config"]["name"];
    searchConfig.version = configFile["config"]["version"];
    searchConfig.maxResponses = configFile["config"]["max_responses"];
    searchConfig.updateTime = configFile["config"]["update_time"];
    searchConfig.files = configFile["files"];

    file.close();
}
// получаем текст документа разбитый на отдельные слова
std::vector<std::string> ConverterJSON::GetTextDocument(size_t docId)
{
    std::vector<std::string> text;
    std::ifstream file(searchConfig.files[docId]);

    if (file.is_open())
    {
        while (!file.eof())
        {
            std::string tempStr;
            file >> tempStr;

            for (int i = 0; i < tempStr.length(); i++)
            {//удаляем символы не являющиеся буквами
                if (!isalpha(tempStr[i]))
                {
                    tempStr.erase(i, 1);
                    i--;
                }
            }
            text.push_back(tempStr);
        }
        file.close();
    }
    else
    {
        std::cout << "File " << searchConfig.files[docId] << " not exists." << std::endl;
    }

    return text;
}

std::vector<std::string> ConverterJSON::GetRequests()
{
    std::ifstream file;
    nlohmann::json requestsFile;
    try
    {
        file.open(".\\Json_files\\requests.json");
        if (!file.is_open())
        {
            throw EngineException(REQUEST_FILE_MISSING);
        }
        file >> requestsFile;
    }
    catch (EngineException &e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
        exit(1);
    }
    std::vector<std::string> requests;
    nlohmann::json::iterator it = requestsFile.begin();

    for (auto &request : requestsFile["requests"])
    {
        for (auto &word : request)
           requests.push_back(word);
    }
    file.clear();
    file.close();

    return requests;
}

void ConverterJSON::PutAnswers(std::vector<std::vector<std::pair<size_t , float>>> answers)
{
    std::ofstream file("..\\Json_files\\answers.json");
    nlohmann::ordered_json answersFile;

    for (int i = 0; i < answers.size(); i++)
    {
        nlohmann::json pair;

        if (answers[i].empty()) //если ответ на запрос пустой
        {
            answersFile["answers"]["request_" + std::to_string(i)]["result"] = "false";
        }
        else //если ответ на запрос есть
        {
            answersFile["answers"]["request_" + std::to_string(i)]["result"] = "true";
            size_t answerSize = answers[i].size();

            for (int j = 0; j < answerSize; j++) // перебираем все документы запроса
            {
                pair["doc_id"] = answers[i][j].first;
                pair["rank"] = std::to_string(answers[i][j].second);

                if (answerSize > 1) // если документов больше одного создаем 'relevance'
                {
                    answersFile["answers"]["request_" + std::to_string(i)]["relevance"].emplace_back(pair);
                }
            }
            if (answerSize == 1)
            {
                answersFile["answers"]["request_" + std::to_string(i)]["dock_id"] = pair["doc_id"];
                answersFile["answers"]["request_" + std::to_string(i)]["rank"] = pair["rank"];
            }
        }
    }
    file << std::setw(2) << answersFile;
    file.close();
}
const SearchConfig &ConverterJSON::GetSearchConfig() const
{
    return searchConfig;
}
