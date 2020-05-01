#pragma once
#include "TextInfo.hpp"

//TODO(vendroid): Нужно реализовать конфиг для базы данных
#include "DataBaseConfig.hpp"

namespace DataBase {
namespace External {

class TextInfoMapper {
public:
  TextInfoMapper();
  std::unique_ptr<TextInfo> GetRandomText();

private:
  std::unique_ptr<mongocxx::collection> _collection = nullptr;

private:
  static constexpr char _kCharCountField[] = "charCount";
  static constexpr char _kTextField[] = "text";
  static constexpr char _kTextIdField[] = "textId";
};

}  // External
}  // DataBase
