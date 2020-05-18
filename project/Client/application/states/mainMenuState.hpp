#pragma once

#include "baseState.hpp"
#include "eventManager.hpp"
#include "stateManager.hpp"
#include "label.hpp"

class MainMenuState : public BaseState {
public:
    explicit MainMenuState(std::weak_ptr<StateManager> stateManager);
    ~MainMenuState() = default;

    void OnCreate() override;
    void OnDestroy() override;

    void Activate() override;
    void Deactivate() override;

    void Update(const sf::Time& l_time) override;
    void Draw() override;

    void MouseClick(EventDetails& l_details);
    void MouseRelease(EventDetails& l_details);
    void Play(EventDetails& l_details);
    void Quit(EventDetails& l_details);

private:
    std::vector<std::shared_ptr<BaseElement>> _elements;
};
