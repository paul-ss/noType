#pragma once

#include "baseState.hpp"
#include "eventManager.hpp"

class PauseState : public BaseState {
    public:
        PauseState(StateManager* stateManager);
        ~PauseState();

        void onCreate() override;
        void onDestroy() override;

        void activate() override;
        void deactivate() override;

        void update(const sf::Time& time) override;
        void draw() override;

        void unpause(EventDetails* details);

    private:
        sf::Font _font;
        sf::Text _text;
        sf::RectangleShape _rect;
};
