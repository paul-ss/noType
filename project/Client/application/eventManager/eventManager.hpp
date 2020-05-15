#pragma once

#include <functional>
#include <unordered_map>
#include <iostream>
#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

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
    EventInfo() : code(0) {}
    EventInfo(int l_event) : code(l_event) {}
    ~EventInfo() {}
    union {
        int code;
    };
};

struct EventDetails {
    EventDetails(const std::string& l_bindName) : name(l_bindName) {
        Clear();
    }

    std::string name;

    sf::Vector2i size;
    sf::Uint32 textEntered;
    sf::Vector2i mouse;
    int mouseWheelDelta;
    int keyCode;

    void Clear() {
        size = sf::Vector2i(0, 0);
        textEntered = 0;
        mouse = sf::Vector2i(0, 0);
        mouseWheelDelta = 0;
        keyCode = -1;
    }
};

using Events = std::vector<std::pair<EventType, EventInfo> >;
struct Binding {
    Binding(const std::string& l_name): name(l_name), count(0), details(l_name) {}
    ~Binding() {}

    void BindEvent(EventType l_type, EventInfo l_info = EventInfo()) {
        events.emplace_back(l_type, l_info);
    }

    Events events;
    std::string name;
    int count;

    EventDetails details;
};

using Bindings = std::unordered_map<std::string, std::shared_ptr<Binding>>;

using CallbackContainer = std::unordered_map<std::string,
        std::function<void(EventDetails&)> >;
enum class StateType;
using Callbacks = std::unordered_map<StateType, CallbackContainer>;

class EventManager {
public:
    EventManager();
    ~EventManager() = default;

    bool AddBinding(std::shared_ptr<Binding> binding);
    bool RemoveBinding(std::string l_name);

    void SetCurrentState(StateType l_state);
    void SetFocus(bool l_focus);

    bool AddCallback(StateType l_state,
                    const std::string& l_name,
                    const std::function<void(EventDetails&)>& l_func) {
        auto itr = _callbacks.emplace(l_state, CallbackContainer()).first;
        return itr->second.emplace(l_name, l_func).second;
    }
    bool RemoveCallback(StateType l_state, const std::string& l_name);

    void HandleEvent(const sf::Event& l_event);
    void Update();

    sf::Vector2i GetMousePos(std::weak_ptr<sf::RenderWindow> l_window);

private:
    void loadBindings();

private:
    Bindings _bindings;
    Callbacks _callbacks;
    StateType _currentState;
    bool _hasFocus;
};
