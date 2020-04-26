#pragma once
#include "baseState.hpp"
#include "eventManager.hpp"

class GameState : public BaseState {
public:
    GameState(StateManager* stateManager);
    ~GameState();

    void onCreate() override;
    void onDestroy() override;

    void activate() override;
    void deactivate() override;

    void update(const sf::Time& time) override;
    void draw() override;

    void mainMenu(EventDetails* details);
    void pause(EventDetails* details);
private:
    sf::Texture _texture;
    sf::Sprite _sprite;
    sf::Vector2f _increment;
};
