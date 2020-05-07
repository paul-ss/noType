#pragma once

#include "window.hpp"
#include "stateManager.hpp"
#include "sharedContext.hpp"

class Game : public sf::NonCopyable {
    public:
        Game();
        ~Game();

        void Run();

    private:
        void update();
        void render();
        void lateUpdate();

    private:
        sf::Time getElapsed();
        void restartClock();

    private:
        sf::Clock _clock;
        sf::Time _elapsed;
        SharedContext _context;
        Window _window;
        TextureManager _textureManager;
        FontManager _fontManager;
        AudioManager _audioManager;
        SoundManager _soundManager;
        GUI_Manager _guiManager;
        StateManager _stateManager;
};
