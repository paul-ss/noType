#pragma once

#include <functional>
#include <unordered_map>

#include <SFML/Graphics.hpp>

enum class EventType {
KeyDown = sf::Event::KeyPressed,
KeyUp = sf::Event::KeyReleased,
MButtonDown = sf::Event::MouseButtonPressed,
MButtonUp = sf::Event::MouseButtonReleased,
MouseWheel = sf::Event::MouseWheelMoved,
WindowResized = sf::Event::Resized,
GainedFocus = sf::Event::GainedFocus,
LostFocus = sf::Event::LostFocus,
MouseEntered = sf::Event::MouseEntered,
MouseLeft = sf::Event::MouseLeft,
Closed = sf::Event::Closed,
TextEntered = sf::Event::TextEntered,
Keyboard = sf::Event::Count + 1, Mouse, Joystick
};

struct EventInfo {
    EventInfo() {
        _code = 0;
    }

    EventInfo(int event) {
        _code = event;
    }

    union{
        int _code;
    };
};

struct EventDetails {
    EventDetails(const std::string& bindName) : _name(bindName) {
        Clear();
    }

    void Clear() {
        _size = sf::Vector2i(0, 0);
        _textEntered = 0;
        _mouse = sf::Vector2i(0, 0);
        _mouseWheelDelta = 0;
        _keyCode = -1;
    }

    std::string _name;
    sf::Vector2i _size;
    sf::Uint32 _textEntered;
    sf::Vector2i _mouse;
    int _mouseWheelDelta;
    int _keyCode;
};

using Events = std::vector<std::pair<EventType, EventInfo> >;
struct Binding {
    Binding(const std::string& name) : _name(name),
                                        _details(name),
                                        _count(0) {
    }

    void BindEvent(EventType type, EventInfo info = EventInfo()) {
        _events.emplace_back(type, info);
    }

    Events _events;
    std::string _name;
    EventDetails _details;
    size_t _count;
};

using Bindings = std::unordered_map<std::string, Binding*>;
using Callbacks = std::unordered_map<std::string, std::function<void(EventDetails*)> >;
class EventManager {
    public:
        EventManager();
        ~EventManager();

        bool addBinding(Binding *binding);
        bool removeBinding(std::string name);
        //void setFocus(const bool& focus);
        void handleEvent(sf::Event& event);
        void update();

        // NOTE(vendroid): Теперь нет смысла делать этот метод шаблонным: просто передаем функциональный объект
        bool addCallback(const std::string& name,
                        const std::function<void(EventDetails*)>& func);
        void removeCallback(const std::string& name);
        sf::Vector2i getMousePos(sf::RenderWindow* wind = nullptr);

    private:
        void loadBindings();

    private:
        Bindings _bindings;
        Callbacks _callbacks;
        bool _hasFocus;
};
