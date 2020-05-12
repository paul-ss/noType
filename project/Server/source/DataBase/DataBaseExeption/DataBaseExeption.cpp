#include "DataBaseExepiton.hpp"

namespace DataBase {
  Exeption::Exeption(std::string&& msg) : _msg(msg) {}

  const char* Exeption::what() const {
    return _msg.c_str();
  }

  InvalidQuery::InvalidQuery(std::string&& msg) : Exeption(msg) {}

  InvalidAuthentication::InvalidAuthentication(std::string&& msg) : Exeption(msg) {}
  
}  // DataBase