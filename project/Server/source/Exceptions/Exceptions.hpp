//
// Created by paul_s on 23.04.2020.
//

#pragma once

#include <exception>
#include <string>

class ServerException: public std::exception {
public:
  explicit ServerException(const std::string& msg)
      : _msg(msg) {}

  const char* what() const noexcept override {
    return _msg.c_str();
  }

private:
  std::string _msg;
};



class RoomManagerException: public ServerException {
public:
  explicit RoomManagerException(const std::string& msg)
      : ServerException("RoomManager-> " + msg) {}
};


class RoomException: public ServerException {
public:
  explicit RoomException(const std::string& msg)
      : ServerException("Room-> " + msg) {}
};


