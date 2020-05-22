#pragma once

#include "baseState.hpp"

enum class ElementName;
struct EventDetails;
class BaseElement;

class BeforeGameState : public BaseState {
public:
    explicit BeforeGameState(std::weak_ptr<SharedContext> l_context);
    ~BeforeGameState() = default;

    void OnCreate() override;
    void OnDestroy() override;

    void Activate() override;
    void Deactivate() override;

    void Update(const sf::Time& time) override;
    void Draw() override;

public:
    void Init();
    void Connect();
    void StartGameSession();
    void GetText();

    void Game();
    void Quit(EventDetails& l_details);

private:
    double _waitTime;
};
