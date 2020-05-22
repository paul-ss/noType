#pragma once

#include "baseState.hpp"

enum class ElementName;
struct EventDetails;
class BaseElement;

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
    double CountAverageSpeed(const double l_speed);
    void CheckRoomStatus();

    void TextEntered(EventDetails& l_details);
    void GoToMenu();

private:
    float _timePass;
    size_t _position;
    double _averageSpeed;
    std::vector<double> _currentSpeed;
};
