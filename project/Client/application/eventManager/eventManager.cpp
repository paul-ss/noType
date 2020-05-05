#include "eventManager.hpp"
#include "exceptions.hpp"

EventManager::EventManager():_currentState(StateType(0)), _hasFocus(true) {
    LoadBindings();
}

void EventManager::SetFocus(const bool& focus) {
    _hasFocus = focus;
}

void EventManager::SetCurrentState(StateType state) {
    _currentState = state;
}

bool EventManager::AddCallback(StateType state,
        const std::string& name,
        const std::function<void(EventDetails&)>& func) {

    auto itr = _callbacks.emplace(state, CallbackContainer()).first;
    return itr->second.emplace(name, func).second;
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

sf::Vector2i EventManager::GetMousePos(const sf::RenderWindow* wind) {
    return (wind ? sf::Mouse::getPosition(*wind) : sf::Mouse::getPosition());
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

void EventManager::HandleEvent(const sf::Event& event) {
    for (auto &b_itr : _bindings) {
        auto bind = b_itr.second;
        for (auto &e_itr : bind->_events) {
            EventType sfmlEvent = (EventType)event.type;
            if (e_itr.first != sfmlEvent) {
                continue;
            }
            if (sfmlEvent == EventType::KeyDown || sfmlEvent == EventType::KeyUp) {
                if (e_itr.second._code == event.key.code) {
                    if (bind->_details._keyCode != -1) {
                        bind->_details._keyCode = e_itr.second._code;
                    }
                    ++(bind->_count);
                    break;
                }
            } else if (sfmlEvent == EventType::MButtonDown || sfmlEvent == EventType::MButtonUp) {
                if (e_itr.second._code == event.mouseButton.button) {
                    bind->_details._mouse.x = event.mouseButton.x;
                    bind->_details._mouse.y = event.mouseButton.y;
                    if (bind->_details._keyCode != -1) {
                        bind->_details._keyCode = e_itr.second._code;
                    }
                    ++(bind->_count);
                    break;
                }
            } else {
                if (sfmlEvent == EventType::MouseWheel) {
                    bind->_details._mouseWheelDelta = event.mouseWheel.delta;
                } else if (sfmlEvent == EventType::WindowResized) {
                    bind->_details._size.x = event.size.width;
                    bind->_details._size.y = event.size.height;
                } else if (sfmlEvent == EventType::TextEntered) {
                    bind->_details._textEntered = event.text.unicode;
                }
                ++(bind->_count);
            }
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

void EventManager::LoadBindings() {
    std::string delimiter = ":";
    std::ifstream bindings;
    bindings.open("assets/keys.cfg");
    if (!bindings.is_open()) {
        throw InvalidFile();
    }

    std::string line;
    while (std::getline(bindings, line)) {
        std::stringstream keystream(line);
        std::string callbackName;
        keystream >> callbackName;
        if (keystream.bad()) {
            throw InvalidFile();
        }

        auto bind = std::make_shared<Binding>(callbackName);
        while (!keystream.eof()) {
            std::string keyval;
            keystream >> keyval;
            if (keystream.bad()) {
                throw InvalidFile();
            }
            size_t start = 0;
            size_t end = keyval.find(delimiter);
            if (end == std::string::npos) {
                break;
            }
            EventType type = EventType(stoi(keyval.substr(start, end - start)));
            int code = stoi(keyval.substr(end + delimiter.length(),
            keyval.find(delimiter, end + delimiter.length())));

            EventInfo eventInfo;
            eventInfo._code = code;
            bind->BindEvent(type, eventInfo);
        }
        if (!AddBinding(bind)) {
            throw InvalidCmd();
        }
    }
    bindings.close();
}
