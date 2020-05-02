#pragma once
#include "RandomName.hpp"

//TODO(vendroid): Нужно реализовать конфиг для базы данных
#include "DataBaseConfig.hpp"

namespace DataBase {
namespace Internal {

class RandomNameGenerator : public IRandomNameGenerator {
public:
  RandomNameGenerator();
  std::unique_ptr<RandomName> GetRandomName();

private:
  std::unique_ptr<mongocxx::collection> _animalNamesCollection = nullptr;
  std::unique_ptr<mongocxx::collection> _colorsCollection = nullptr;

private:
  static constexpr char _kAnimalNameField[] = "AnimalName";
  static constexpr char _kAnimalIdField[] = "AnimalId";

  static constexpr char _kColorNameField[] = "ColorName";
  static constexpr char _kColorIdField[] = "ColorId";
};

}  // Internal
}  // DataBase
