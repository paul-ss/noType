#pragma once

#include <unordered_map>

#include "baseState.hpp"
#include "Network.hpp"

enum class ElementName;
struct EventDetails;
class BaseElement;

namespace Network {
  struct PlayerInfo;
}

class GameState : public BaseState {
public:
    explicit GameState(std::weak_ptr<SharedContext> l_context);
    ~GameState() = default;

    void OnCreate() override;
    void OnDestroy() override;

    void Activate() override;
    void Deactivate() override;

    void Update(const sf::Time& l_time) override;
    void Draw() override;

public:
    void GetAverageSpeed();
    double CountAverageSpeed(const double l_speed);

    void UpdateLeaderPosition(const std::unordered_map<std::string, Network::PlayerInfo>& l_players);
    void CheckRoomStatus();
    void UpdatePosition(const std::string& l_validatedBlock);

    size_t UpdatePlayerPosition(const std::unordered_map<std::string, Network::PlayerInfo>& l_players);
    void GetPlayerPosition();

    void TextEntered(EventDetails& l_details);
    void AfterGame();
    void Menu();

private:
    size_t _textPosition;
    size_t _playerPosition;
    std::string _gameCookie;
    float _timePass;
    double _averageSpeed;
    std::vector<double> _currentSpeed;
};
