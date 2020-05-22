#pragma once

#include "baseState.hpp"

enum class ElementName;
struct EventDetails;
class BaseElement;

class IntroState : public BaseState {
public:
    explicit IntroState(std::weak_ptr<SharedContext> l_context);
    ~IntroState() = default;

    void OnCreate() override;
    void OnDestroy() override;

    void Activate() override;
    void Deactivate() override;

    void Update(const sf::Time& time) override;
    void Draw() override;

public:
    void Continue(EventDetails& details);
};
