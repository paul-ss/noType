#pragma once

#include "eventManager.hpp"
#include "baseState.hpp"
#include "stateManager.hpp"

class IntroState : public BaseState {
    public:
        IntroState(std::shared_ptr<StateManager> stateManager);
        ~IntroState();

        void onCreate();
        void onDestroy();

        void activate();
        void deactivate();

        void update(const sf::Time& time);
        void draw();

        void proceed(std::shared_ptr<EventDetails> details);

    private:
        sf::Texture _introTexture;
        sf::Sprite _introSprite;
        sf::Font _font;
        sf::Text _text;

    private:
        float _timePassed;
};
