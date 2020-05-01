#pragma once

#include <string>

namespace DataBase {
namespace External {

struct TextInfo {
    size_t charCount;
    std::string text;
};

class ITextInfoMapper {
public:
    virtual std::unique_ptr<TextInfo> GetRandomText()  = 0;
};

}  // External
}  // DataBase
