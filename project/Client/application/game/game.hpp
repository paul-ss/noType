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

private:
    std::shared_ptr<SharedContext> _context;
    std::shared_ptr<Window> _window;
    std::shared_ptr<TextureManager> _textureManager;
    std::shared_ptr<FontManager> _fontManager;
    std::shared_ptr<AudioManager> _audioManager;
    std::shared_ptr<SoundManager> _soundManager;
    std::shared_ptr<GUI_Manager> _guiManager;
    std::shared_ptr<StateManager> _stateManager;
};
