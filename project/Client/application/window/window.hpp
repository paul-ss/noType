#pragma once

#include "eventManager.hpp"

#include <SFML/Graphics.hpp>

class Window {
    public:
        Window();
        explicit Window(const std::string& title, const sf::Vector2u& size);
        ~Window();

        void BeginDraw();
        void Draw(sf::Drawable& drawable);
        void EndDraw();
        void Update();
        void Close();
        void ToggleFullscreen();
        bool IsDone();
        bool IsFocused();
        bool IsFullScreen();
        sf::Vector2u GetWindowSize();
        EventManager* GetEventManager();
        sf::RenderWindow* GetRenderWindow();

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
