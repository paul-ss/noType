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
    void CountAverageSpeed(const double l_speed);

    //void UpdateLeaderPosition(const std::unordered_map<std::string, Network::PlayerInfo>& l_players);
    void CheckRoomStatus();
    void UpdatePosition(const std::string& l_validatedBlock);

    void UpdatePosition(const std::unordered_map<std::string, Network::PlayerInfo>& l_players);
    void GetPlayerPosition();

    void TextEntered(EventDetails& l_details);
    void AfterGame();
    void Menu();

private:
    void refreshBar(std::pair<std::string, Network::PlayerInfo>& l_position, ElementName l_elementName);

private:
    size_t _textPosition;
    size_t _playerPosition;
    std::string _gameCookie;
    float _timePass;
    double _averageSpeed;
    std::vector<double> _currentSpeed;
};
