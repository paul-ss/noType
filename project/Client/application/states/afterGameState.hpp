#pragma once

#include "baseState.hpp"
#include "eventManager.hpp"
#include "stateManager.hpp"
#include "label.hpp"

class AfterGameState : public BaseState {
public:
    explicit AfterGameState(std::weak_ptr<StateManager> l_stateManager);
    ~AfterGameState() = default;

    void OnCreate() override;
    void OnDestroy() override;

    void Activate() override;
    void Deactivate() override;

    void Update(const sf::Time& l_time) override;
    void Draw() override;

    void MainMenu(EventDetails& l_details);
    void Game(EventDetails& l_details);

private:
    std::vector<std::shared_ptr<BaseElement>> _elements;
};
