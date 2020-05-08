#include <filesystem>

#include "eventManager.hpp"
#include "exceptions.hpp"
#include "logger.hpp"

EventManager::EventManager():_currentState(StateType(0)), _hasFocus(true) {
    loadBindings();
}

void EventManager::SetFocus(bool focus) {
    _hasFocus = focus;
}

void EventManager::SetCurrentState(StateType state) {
    _currentState = state;
}

bool EventManager::RemoveCallback(StateType state, const std::string& name) {
    auto itr = _callbacks.find(state);
    if (itr == _callbacks.end()) {
        return false;
    }

    auto itr2 = itr->second.find(name);
    if (itr2 == itr->second.end()) {
        return false;
    }
    itr->second.erase(name);
    return true;
}

sf::Vector2i EventManager::GetMousePos(std::weak_ptr<sf::RenderWindow> l_window) {
    return (l_window.lock() ? sf::Mouse::getPosition(*l_window.lock()) : sf::Mouse::getPosition());
}

bool EventManager::AddBinding(std::shared_ptr<Binding> binding) {
    if (_bindings.find(binding->_name) != _bindings.end()) {
        return false;
    }
    return _bindings.emplace(binding->_name, binding).second;
}

bool EventManager::RemoveBinding(std::string name) {
    auto itr = _bindings.find(name);
    if (itr == _bindings.end()) {
        return false;
    }
    _bindings.erase(itr);
    return true;
}

void EventManager::HandleEvent(const sf::Event& l_event) {
    for (auto &b_itr : _bindings) {
        auto bind = b_itr.second;
        for (auto &e_itr : bind->_events) {
            EventType sfmlEvent = (EventType)l_event.type;
            if (e_itr.first != sfmlEvent) {
                continue;
            }
            if (sfmlEvent == EventType::KeyDown || sfmlEvent == EventType::KeyUp) {
                if (e_itr.second._code == l_event.key.code) {
                    if (bind->_details._keyCode != -1) {
                        bind->_details._keyCode = e_itr.second._code;
                    }
                    ++(bind->_count);
                    break;
                }
            } else if (sfmlEvent == EventType::MButtonDown || sfmlEvent == EventType::MButtonUp) {
                if (e_itr.second._code == l_event.mouseButton.button) {
                    bind->_details._mouse.x = l_event.mouseButton.x;
                    bind->_details._mouse.y = l_event.mouseButton.y;
                    if (bind->_details._keyCode != -1) {
                        bind->_details._keyCode = e_itr.second._code;
                    }
                    ++(bind->_count);
                    break;
                }
            } else {
                if (sfmlEvent == EventType::MouseWheel) {
                    bind->_details._mouseWheelDelta = l_event.mouseWheel.delta;
                } else if (sfmlEvent == EventType::WindowResized) {
                    bind->_details._size.x = l_event.size.width;
                    bind->_details._size.y = l_event.size.height;
                } else if (sfmlEvent == EventType::TextEntered) {
                    bind->_details._textEntered = l_event.text.unicode;
                }
                ++(bind->_count);
            }
        }
    }
}

void EventManager::HandleEvent(const GUI_Event& l_event) {
    for (auto &b_itr : _bindings) {
        auto bind = b_itr.second;
        for (auto &e_itr : bind->_events) {
            if (e_itr.first != EventType::GUI_Click && e_itr.first != EventType::GUI_Release &&
                e_itr.first != EventType::GUI_Hover && e_itr.first != EventType::GUI_Leave) {
                continue;
            }
            if ((e_itr.first == EventType::GUI_Click && l_event._type != GUI_EventType::Click) ||
                (e_itr.first == EventType::GUI_Release && l_event._type != GUI_EventType::Release) ||
                (e_itr.first == EventType::GUI_Hover && l_event._type != GUI_EventType::Hover) ||
                (e_itr.first == EventType::GUI_Leave && l_event._type != GUI_EventType::Leave)) {
                    continue;
            }
            if ((e_itr.second._gui._interface == l_event._interface) ||
                (e_itr.second._gui._element == l_event._element)) {
                    continue;
            }
            bind->_details._guiInterface = l_event._interface;
            bind->_details._guiElement = l_event._element;
            ++(bind->_count);
        }
    }
}

void EventManager::Update() {
    if (!_hasFocus) {
        return;
    }
    for (auto &b_itr : _bindings) {
        auto bind = (b_itr.second);
        for (auto &e_itr : bind->_events) {
            switch (e_itr.first) {
                case(EventType::Keyboard) : {
                    if (sf::Keyboard::isKeyPressed( sf::Keyboard::Key(e_itr.second._code))) {
                        if (bind->_details._keyCode != -1) {
                            bind->_details._keyCode = e_itr.second._code;
                        }
                        ++(bind->_count);
                    }
                    break;
                }

                case(EventType::Mouse) : {
                    if (sf::Mouse::isButtonPressed( sf::Mouse::Button(e_itr.second._code))) {
                        if (bind->_details._keyCode != -1) {
                            bind->_details._keyCode = e_itr.second._code;
                        }
                        ++(bind->_count);
                    }
                    break;
                }

                default: {
                }
            }
        }

        if (bind->_events.size() == bind->_count) {
            auto stateCallbacks = _callbacks.find(_currentState);
            auto otherCallbacks = _callbacks.find(StateType(0));
            if (stateCallbacks != _callbacks.end()) {
                auto callItr = stateCallbacks->second.find(bind->_name);
                if (callItr != stateCallbacks->second.end()) {
                    callItr->second(bind->_details);
                }
            }
            if (otherCallbacks != _callbacks.end()) {
                auto callItr = otherCallbacks->second.find(bind->_name);
                if (callItr != otherCallbacks->second.end()) {
                    callItr->second(bind->_details);
                }
            }
        }

        bind->_count = 0;
        bind->_details.Clear();
    }
}

void EventManager::loadBindings() {
    std::string delimiter = ":";
    std::ifstream bindings;
    std::filesystem::path path = std::filesystem::absolute("assets/keys.cfg");
    bindings.open(path);
    if (!bindings.is_open()) {
        BOOST_LOG_TRIVIAL(error) << "Failed to load keys file: " << path;
        return;
    }
    std::string line;
    while(std::getline(bindings, line)) {
        std::stringstream keystream(line);
        std::string callbackName;
        keystream >> callbackName;
        auto bind = std::make_shared<Binding>(callbackName);
        while(!keystream.eof()) {
            std::string keyval;
            keystream >> keyval;
            int start = 0;
            int end = keyval.find(delimiter);
            if (end == std::string::npos) {
                break;
            }
            EventType type = EventType(stoi(keyval.substr(start, end-start)));

            EventInfo eventInfo;
            if (type == EventType::GUI_Click || type == EventType::GUI_Release ||
                type == EventType::GUI_Hover || type == EventType::GUI_Leave) {
                start = end + delimiter.length();
                end = keyval.find(delimiter, start);
                std::string window = keyval.substr(start, end - start);
                std::string element;
                if (end != std::string::npos) {
                    start = end + delimiter.length();
                    end = keyval.length();
                    element = keyval.substr(start, end);
                }

                eventInfo._gui._interface = window;
                eventInfo._gui._element = element;
            } else {
                int code = stoi(keyval.substr(end + delimiter.length(),
                    keyval.find(delimiter,end + delimiter.length())));
                eventInfo._code = code;
            }
            bind->BindEvent(type, eventInfo);
        }

        if (!AddBinding(bind)) {
            BOOST_LOG_TRIVIAL(error) << "Failed to add new Binding";
            throw InvalidCmd();
        }
    }
    bindings.close();
}
