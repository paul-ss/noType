#pragma once

#include <window.hpp>

class Game {
    public:
        Game();
        ~Game() = default;

        Game(Game& other) = delete;
        void operator=(const Game&) = delete;

    public:
        void run();

    private:
        void update();
        void render();

    private:
        Window _window;
};
