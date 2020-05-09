#pragma once

#include "baseState.hpp"
#include "eventManager.hpp"
#include "stateManager.hpp"

class MainMenuState : public BaseState{
public:
    explicit MainMenuState(std::weak_ptr<StateManager> stateManager);
    ~MainMenuState() = default;

    void OnCreate() override;
    void OnDestroy() override;

    void Activate() override;
    void Deactivate() override;

    void Update(const sf::Time& time) override;
    void Draw() override;

    void MouseClick(EventDetails& details);
    void Play(EventDetails& details);
    void Quit(EventDetails& details);
};
