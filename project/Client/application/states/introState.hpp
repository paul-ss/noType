#pragma once

#include "eventManager.hpp"
#include "baseState.hpp"
#include "stateManager.hpp"

class IntroState : public BaseState {
    public:
        explicit IntroState(StateManager* stateManager);
        ~IntroState();

        void OnCreate() override;
        void OnDestroy() override;

        void Activate() override;
        void Deactivate() override;

        void Update(const sf::Time& time) override;
        void Draw() override;

        void Continue(EventDetails* details);

    private:
        sf::Sprite _introSprite;
        sf::Text _text;
};
