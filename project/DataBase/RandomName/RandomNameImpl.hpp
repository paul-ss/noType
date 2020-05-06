#pragma once
#include "RandomName.hpp"

//TODO(vendroid): Нужно реализовать конфиг для базы данных
#include "DataBaseConfig.hpp"

namespace DataBase {
namespace Internal {

class RandomNameGenerator : public IRandomNameGenerator {
public:
  RandomNameGenerator();
  RandomNameGenerator(const std::string& dataBaseName);
  std::unique_ptr<RandomName> GetRandomName();

private:
  std::string _dataBaseName;

private:
  std::string get_random_document_by_id_and_field(
    const char* collectionName, const char* idField, const char* fieldName) const;

private:
  static constexpr char _kAnimalNameField[] = "AnimalName";
  static constexpr char _kAnimalIdField[] = "AnimalId";

  static constexpr char _kColorNameField[] = "ColorName";
  static constexpr char _kColorIdField[] = "ColorId";
};

}  // Internal
}  // DataBase
