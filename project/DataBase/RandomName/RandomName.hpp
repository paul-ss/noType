#pragma once

#include <string>

namespace DataBase {
namespace Internal {

struct RandomName {
  RandomName(std::string&& name) : name(name) {}
  std::string name;
};

class IRandomNameGenerator {
public:
  virtual std::unique_ptr<RandomName> GetRandomName() = 0;
  virtual ~IRandomNameGenerator() = default;
};

}  // Internal
}  // DataBase
