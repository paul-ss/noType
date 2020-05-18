#include "DataBaseException.hpp"

namespace DataBase {

Exception::Exception(std::string&& msg) noexcept : _msg(msg) {}

const char* Exception::what() const noexcept {
  return _msg.c_str();
}

}  // DataBase