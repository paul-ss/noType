#pragma once

#include "window.hpp"
#include "stateManager.hpp"
#include "sharedContext.hpp"

class Game : public sf::NonCopyable {
    public:
        Game();
        ~Game() = default;

    public:
        void run();

    private:
        void update();
        void render();
        void lateUpdate();

    private:
        sf::Time getElapsed();
        void restartClock();

    private:
        Window _window;
        SharedContext _context;
        StateManager _stateManager;
        AudioManager _audioManager;
        SoundManager _soundManager;
        sf::Clock _clock;
        sf::Time _elapsed;
};
