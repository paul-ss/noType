#pragma once

#include "baseState.hpp"
#include "eventManager.hpp"
#include "stateManager.hpp"

class AfterGameState : public BaseState {
    public:
        explicit AfterGameState(StateManager* l_stateManager);
        ~AfterGameState();

        void OnCreate() override;
        void OnDestroy() override;

        void Activate() override;
        void Deactivate() override;

        void Update(const sf::Time& l_time) override;
        void Draw() override;

        void MainMenu(EventDetails* l_details);

    private:
        sf::Texture _texture;
        sf::Sprite _sprite;
        sf::Vector2f _increment;
};