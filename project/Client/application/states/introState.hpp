#pragma once

#include "eventManager.hpp"
#include "baseState.hpp"
#include "stateManager.hpp"

class IntroState : public BaseState {
    public:
        IntroState(StateManager* stateManager);
        ~IntroState();

        void OnCreate() override;
        void OnDestroy() override;

        void Activate() override;
        void Deactivate() override;

        void Update(const sf::Time& time) override;
        void Draw() override;

        void Continue(EventDetails* details);

    private:
        sf::Texture _introTexture;
        sf::Sprite _introSprite;
        sf::Font _font;
        sf::Text _text;

    private:
        float _timePassed;
};
