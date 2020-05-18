#pragma once

#include "baseState.hpp"
#include "eventManager.hpp"
#include "stateManager.hpp"
#include "label.hpp"

class IntroState : public BaseState {
public:
    explicit IntroState(std::weak_ptr<StateManager> stateManager);
    ~IntroState() = default;

    void OnCreate() override;
    void OnDestroy() override;

    void Activate() override;
    void Deactivate() override;

    void Update(const sf::Time& time) override;
    void Draw() override;

    void Continue(EventDetails& details);

private:
    std::vector<std::shared_ptr<BaseElement>> _elements;
};
