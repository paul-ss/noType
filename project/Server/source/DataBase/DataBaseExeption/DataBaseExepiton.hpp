#pragma once

#include <exception>
#include <string>

namespace DataBase {

class Exeption : public std::exception {
public:
  Exeption(std::string&& msg) noexcept;
  virtual ~Exeption() noexcept = default;

  const char* what() const noexcept override;

protected:
  std::string _msg;
};

class InvalidQuery : public Exeption {
public:
  InvalidQuery(std::string&& msg) noexcept;
};

class InvalidAuthentication : public Exeption {
public:
  InvalidAuthentication(std::string&& msg) noexcept;
};

}  // namespace DataBase
