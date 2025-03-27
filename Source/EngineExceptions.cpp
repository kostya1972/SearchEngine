//
// Created by Kostya on 16.01.2025.
//

#include "../Include/EngineExceptions.h"

EngineException::EngineException(int i)
{
   if (i == CONF_FILE_MISSING)
       message = "Config file is missing.";
   else if (i == CONF_FILE_EMPTY)
       message = "Config file is empty.";
   else if (i == REQUEST_FILE_MISSING)
       message = "Requests file is missing.";
}
