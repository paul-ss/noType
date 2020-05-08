#pragma once

#include <functional>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "GUI_event.hpp"

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
Keyboard = sf::Event::Count + 1, Mouse, Joystick,
        GUI_Click, GUI_Release, GUI_Hover, GUI_Leave
};

struct EventInfo {
    EventInfo() : _code(0) {}
    EventInfo(int l_event) : _code(l_event) {}
    EventInfo(const GUI_Event& l_guiEvent) : _gui(l_guiEvent) {}
    EventInfo(const EventInfo& l_eventInfo) : _gui(l_eventInfo._gui) {}
    ~EventInfo() {}
    union {
        int _code;
        GUI_Event _gui;
    };
};

struct EventDetails {
    EventDetails(const std::string& l_bindName) : _name(l_bindName) {
        Clear();
    }

    std::string _name;

    sf::Vector2i _size;
    sf::Uint32 _textEntered;
    sf::Vector2i _mouse;
    int _mouseWheelDelta;
    int _keyCode;

    std::string _guiInterface;
    std::string _guiElement;
    GUI_EventType _guiEvent;

    void Clear() {
        _size = sf::Vector2i(0, 0);
        _textEntered = 0;
        _mouse = sf::Vector2i(0, 0);
        _mouseWheelDelta = 0;
        _keyCode = -1;
        _guiInterface = "";
        _guiElement = "";
        _guiEvent = GUI_EventType::None;
    }
};

using Events = std::vector<std::pair<EventType, EventInfo> >;
struct Binding {
    Binding(const std::string& l_name): _name(l_name), _details(l_name), _count(0) {}
    ~Binding() {}

    void BindEvent(EventType l_type, EventInfo l_info = EventInfo()) {
        _events.emplace_back(l_type, l_info);
    }

    Events _events;
    std::string _name;
    int _count;

    EventDetails _details;
};

using Bindings = std::unordered_map<std::string, std::shared_ptr<Binding>>;

using CallbackContainer = std::unordered_map<std::string,
        std::function<void(EventDetails&)> >;
enum class StateType;
using Callbacks = std::unordered_map<StateType, CallbackContainer>;

class EventManager {
public:
    EventManager();
    ~EventManager();

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
    void HandleEvent(const GUI_Event& l_event);
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
