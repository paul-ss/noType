#pragma once

#include <mongocxx/instance.hpp>

namespace DataBase {

class MongoCxxInit {
public:
  static MongoCxxInit& Instance();

private:
  MongoCxxInit();

private:
  mongocxx::instance inst;
};

}  // DataBase