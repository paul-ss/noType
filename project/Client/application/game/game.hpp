#pragma once

#include <window.hpp>

class Game : public sf::NonCopyable {
    public:
        Game();
        ~Game() = default;

    public:
        void run();

    private:
        void update();
        void render();

    private:
        Window _window;
};
