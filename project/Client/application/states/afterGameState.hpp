#pragma once

#include "baseState.hpp"

enum class ElementName;
struct EventDetails;
class BaseElement;

class AfterGameState : public BaseState {
public:
    explicit AfterGameState(std::weak_ptr<SharedContext> l_context);
    ~AfterGameState() = default;

    void OnCreate() override;
    void OnDestroy() override;

    void Activate() override;
    void Deactivate() override;

    void Update(const sf::Time& l_time) override;
    void Draw() override;

public:
    void MainMenu(EventDetails& l_details);
    void Game(EventDetails& l_details);
};
