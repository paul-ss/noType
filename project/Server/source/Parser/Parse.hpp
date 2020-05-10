#pragma once
#include <iostream>
#include <map>
#include "../../rapidjson/document.h"
#include "../../rapidjson/writer.h"
#include "../../rapidjson/stringbuffer.h"

class parsableFromJSON{
 public:
    virtual int parseFromJSON(const std::string &data);
};

class parsableToJSON{
 public:
    virtual std::string parseToJSON() = 0;
};
