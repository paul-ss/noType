//
// Created by paul_s on 23.04.2020.
//

#pragma once

#include "Error.hpp"

#include <unordered_map>
#include <variant>




template <class T, class Error>
class Expected {
public:
  Expected() : _data(std::move(T())) {}
  Expected(const Expected &exp) : _data(exp) {}
  Expected(Expected &&exp) : _data(std::move(exp)) {}
  Expected(T &&t) : _data(std::move(t)) {}
  Expected(const T &t) : _data(t) {}
  Expected(Error &&err) : _data(std::move(err)) {}
  Expected(const Error &err) : _data(err) {}


  operator bool() const {
    return std::holds_alternative<T>(_data);
  }

  // can throw std::bad_variant_access
  auto &value() {
    return  std::get<T>(_data);
  }

  // can throw std::bad_variant_access
  auto &error() {
    return  std::get<Error>(_data);
  }

private:
  std::variant <T, Error> _data;
};

