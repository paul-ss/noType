#pragma once

#include <exception>
#include <string>

namespace DataBase {

class Exception : public std::exception {
public:
  Exception(std::string&& msg) noexcept;

  virtual ~Exception() noexcept = default;

  const char* what() const noexcept override;

private:
  std::string _msg;
};

}  // namespace DataBase
