#pragma once

#include "baseState.hpp"
#include "eventManager.hpp"

class MainMenuState : public BaseState{
    public:
        MainMenuState(StateManager* stateManager);
        ~MainMenuState();

        void onCreate();
        void onDestroy();

        void activate();
        void deactivate();

        void update(const sf::Time& time);
        void draw();

        void mouseClick(EventDetails* details);

    private:
        sf::Font font;
        sf::Text text;

        sf::Vector2f buttonSize;
        sf::Vector2f buttonPos;
        unsigned int buttonPadding;

        sf::RectangleShape rects[3];
        sf::Text labels[3];
};
