#pragma once
#include "baseState.hpp"
#include "eventManager.hpp"

class GameState : public BaseState {
    public:
        GameState(StateManager* stateManager);
        ~GameState();

        void OnCreate() override;
        void OnDestroy() override;

        void Activate() override;
        void Deactivate() override;

        void Update(const sf::Time& time) override;
        void Draw() override;

        void MainMenu(EventDetails* details);

    private:
        sf::Texture _texture;
        sf::Sprite _sprite;
        sf::Vector2f _increment;
};
