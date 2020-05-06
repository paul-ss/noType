#pragma once

#include "TextInfo.hpp"

//TODO(vendroid): Нужно реализовать конфиг для базы данных
#include "DataBaseConfig.hpp"

#include <memory>

namespace DataBase {
namespace External {

class TextInfoMapper : public ITextInfoMapper {
public:
  TextInfoMapper();
  TextInfoMapper(const std::string& dataBaseName);
  std::unique_ptr<TextInfo> GetRandomText();

private:
  std::string _dataBaseName;

private:
  static constexpr char _kTextIdField[] = "TextId";
  static constexpr char _kCharCountField[] = "CharCount";
  static constexpr char _kTextField[] = "Text";
};

}  // External
}  // DataBase
