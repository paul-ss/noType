#pragma once

#include <string>

namespace Network {

struct PlayerInfo {
  std::string name;
  std::uint16_t position;
  double speed;
  enum class Status {
    Play,
    Finish,
    Win
  };
  Status status;
};

}  // namespace Network
