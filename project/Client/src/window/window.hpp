#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

class EventManager;

class Window {
public:
    Window() = delete;
    explicit Window(const std::string& l_title);
    ~Window();

    void BeginDraw();
    void Draw(sf::Drawable& l_drawable);
    void EndDraw();
    void Update();
    void Close();
    bool IsDone();
    bool IsFocused();
    std::weak_ptr<EventManager> GetEventManager();
    std::weak_ptr<sf::RenderWindow> GetRenderWindow();

private:
    void setup(const std::string& l_title);
    void destroy();
    void create();

private:
    std::shared_ptr<sf::RenderWindow> _window;
    std::string _windowTitle;
    std::shared_ptr<EventManager> _eventManager;
    bool _isFocused;
    bool _isDone;
};
