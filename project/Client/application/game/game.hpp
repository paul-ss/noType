#pragma once

#include <window.hpp>

class Game {
    public:
        void run();
        static Game& getInstance();

    private:
        Game();
        void update();
        void render();

    private:
        Window _window;
};
