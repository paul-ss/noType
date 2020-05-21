#pragma once

#include <memory>

#include <SFML/System.hpp>

struct SharedContext;
class Window;
class TextureManager;
class FontManager;
class AudioManager;
class SoundManager;
class StateManager;

namespace Network {
    class INetworkManager;
    namespace Client {
        class IQueueManager;
    }
}

class Game {
public:
    Game();
    ~Game() = default;

    Game(const Game& l_game) = delete;
    Game& operator=(const Game& l_game) = delete;

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
    std::shared_ptr<StateManager> _stateManager;
    std::shared_ptr<Network::Client::IQueueManager> _queueManager;
    std::shared_ptr<Network::INetworkManager> _networkManager;
};
