//
// Created by paul_s on 23.04.2020.
//

#pragma once

#include <string>

struct ErrorMsg {
  ErrorMsg() = default;
  explicit ErrorMsg(const std::string &s) : _msg(s) {}
  explicit ErrorMsg(std::string &&s) : _msg(std::move(s)) {}
  virtual ~ErrorMsg() = default;
  virtual  std::string what() = 0;

protected:
  std::string _msg;
};


struct RoomError : public ErrorMsg {
  explicit RoomError(const std::string &s) : ErrorMsg(s) {}
  explicit RoomError(std::string &&s) : ErrorMsg(std::move(s)) {}
  std::string what() override {
    return "RoomError: " + _msg;
  }
};
