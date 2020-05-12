#pragma once

#include "DataBaseException.hpp"

#include <string>
#include <memory>

namespace DataBase {
namespace External {

struct TextInfo {
    TextInfo(size_t charCount, std::string&& text) 
        : charCount(charCount), text(text) {}
    size_t charCount;
    std::string text;
};

class ITextInfoMapper {
public:
    virtual std::unique_ptr<TextInfo> GetRandomText() = 0;
    virtual ~ITextInfoMapper() = default;
};

}  // External
}  // DataBase
