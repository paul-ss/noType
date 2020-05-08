#pragma once

#include <SFML/Graphics.hpp>

#include "eventManager.hpp"

class Window {
public:
    Window();
    explicit Window(const std::string& l_title, const sf::Vector2u& l_size);
    ~Window();

    void BeginDraw();
    void Draw(sf::Drawable& l_drawable);
    void EndDraw();
    void Update();
    void Close();
    void ToggleFullscreen();
    bool IsDone();
    bool IsFocused();
    bool IsFullScreen();
    sf::Vector2u GetWindowSize();
    std::weak_ptr<EventManager> GetEventManager();
    std::weak_ptr<sf::RenderWindow> GetRenderWindow();

private:
    void setup(const std::string& l_title, const sf::Vector2u& l_size);
    void destroy();
    void create();

private:
    std::shared_ptr<sf::RenderWindow> _window;
    sf::Vector2u _windowSize;
    std::string _windowTitle;
    std::shared_ptr<EventManager> _eventManager;
    bool _isFocused;
    bool _isDone;
    bool _isFullScreen;
};
