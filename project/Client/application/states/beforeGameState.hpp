#pragma once

#include "baseState.hpp"
#include "eventManager.hpp"
#include "stateManager.hpp"
#include "label.hpp"

class BeforeGameState : public BaseState {
public:
    explicit BeforeGameState(std::weak_ptr<StateManager> stateManager);
    ~BeforeGameState();

    void OnCreate() override;
    void OnDestroy() override;

    void Activate() override;
    void Deactivate() override;

    void Update(const sf::Time& time) override;
    void Draw() override;

public:
    void Quit(EventDetails& l_details);

private:
    std::vector<std::shared_ptr<BaseElement>> _elements;
};
