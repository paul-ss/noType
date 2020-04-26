#pragma once

#include "baseState.hpp"
#include "eventManager.hpp"

class MainMenuState : public BaseState{
    public:
        MainMenuState(StateManager* stateManager);
        ~MainMenuState();

        void onCreate() override;
        void onDestroy() override;

        void activate() override;
        void deactivate() override;

        void update(const sf::Time& time) override;
        void draw() override;

        void mouseClick(EventDetails* details);

    private:
        sf::Font _font;
        sf::Text _text;

        sf::Vector2f _buttonSize;
        sf::Vector2f _buttonPos;
        unsigned int _buttonPadding;

        sf::RectangleShape _rects[3];
        sf::Text _labels[3];
};
