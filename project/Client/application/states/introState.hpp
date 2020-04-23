#pragma once

#include "eventManager.hpp"
#include "baseState.hpp"
#include "stateManager.hpp"

class IntroState : public BaseState {
    public:
        IntroState(StateManager* stateManager);
        ~IntroState();

        void onCreate() override;
        void onDestroy() override;

        void activate() override;
        void deactivate() override;

        void update(const sf::Time& time) override;
        void draw() override;

        void proceed(EventDetails* details);

    private:
        sf::Texture _introTexture;
        sf::Sprite _introSprite;
        sf::Font _font;
        sf::Text _text;

    private:
        float _timePassed;
};
