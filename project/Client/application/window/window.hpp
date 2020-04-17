#pragma once

#include "eventManager.hpp"

#include <SFML/Graphics.hpp>

class Window {
    public:
        Window();
        Window(const std::string& title, const sf::Vector2u& size);
        ~Window();
        void beginDraw();
        void draw(sf::Drawable& drawable);
        void endDraw();
        void update();
        void close();
        bool isDone();
        bool isFullScreen();
        bool isFocused();
        sf::Vector2u getWindowSize();
        EventManager* getEventManager();
        void toggleFullscreen(EventDetails* details);

    private:
        void setup(const std::string& title, const sf::Vector2u& size);
        void destroy();
        void create();

    private:
        sf::RenderWindow _window;
        sf::Vector2u _windowSize;
        std::string _windowTitle;
        EventManager _eventManager;
        bool _isFocused;
        bool _isDone;
        bool _isFullScreen;
};