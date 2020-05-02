#pragma once

#include <string>

namespace DataBase {
namespace Internal {

struct RandomName {
  std::string name;
};

class IRandomNameGenerator {
public:
  virtual std::unique_ptr<RandomName> GetRandomName() = 0;
};

}  // Internal
}  // DataBase
