#pragma once

#include "baseState.hpp"

enum class ElementName;
struct EventDetails;
class BaseElement;

class MainMenuState : public BaseState {
public:
    explicit MainMenuState(std::weak_ptr<SharedContext> l_context);
    ~MainMenuState() = default;

    void OnCreate() override;
    void OnDestroy() override;

    void Activate() override;
    void Deactivate() override;

    void Update(const sf::Time& l_time) override;
    void Draw() override;

public:
    void MouseClick(EventDetails& l_details);
    void MouseRelease(EventDetails& l_details);

    void Play();
    void Quit();
};
