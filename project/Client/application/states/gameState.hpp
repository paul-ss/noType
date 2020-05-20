#pragma once

#include "baseState.hpp"
#include "eventManager.hpp"
#include "stateManager.hpp"
#include "smartString.hpp"

class GameState : public BaseState {
public:
    explicit GameState(std::weak_ptr<StateManager> stateManager);
    ~GameState() = default;

    void OnCreate() override;
    void OnDestroy() override;

    void Activate() override;
    void Deactivate() override;

    void Update(const sf::Time& l_time) override;
    void Draw() override;

    void TextEntered(EventDetails& l_details);
    void MainMenu(EventDetails& l_details);

private:
    std::unordered_map<std::string, std::shared_ptr<BaseElement>> _elements;
};
