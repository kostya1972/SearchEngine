//
// Created by Kostya on 16.01.2025.
//

#ifndef SEARCHENGINE_ENGINEEXCEPTIONS_H
#define SEARCHENGINE_ENGINEEXCEPTIONS_H

#include <exception>
#include <string>

const int CONF_FILE_MISSING    =  0;
const int CONF_FILE_EMPTY      =  1;
const int REQUEST_FILE_MISSING =  2;

class EngineException : public std::exception
{
public:

    EngineException() = default;
    EngineException(int);
    const char *what() const noexcept override {return message;}

private:

    const char *message = nullptr;
};


#endif //SEARCHENGINE_ENGINEEXCEPTIONS_H
