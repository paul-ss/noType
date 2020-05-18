#pragma once

#include "baseState.hpp"
#include "eventManager.hpp"

class MainMenuState : public BaseState{
    public:
        MainMenuState(StateManager* stateManager);
        ~MainMenuState();

        void OnCreate() override;
        void OnDestroy() override;

        void Activate() override;
        void Deactivate() override;

        void Update(const sf::Time& time) override;
        void Draw() override;

        void MouseClick(EventDetails* details);
        void Play(EventDetails* details);
        void Quit(EventDetails* details);

    private:
        sf::Font _font;
        sf::Text _text;

        sf::Vector2f _buttonSize;
        sf::Vector2f _buttonPos;
        unsigned int _buttonPadding;

        sf::RectangleShape _rects[3];
        sf::Text _labels[3];
};
