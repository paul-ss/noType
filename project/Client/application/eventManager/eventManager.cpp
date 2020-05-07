#include "eventManager.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

EventManager::EventManager(): _hasFocus(true) {
    loadBindings();
}

EventManager::~EventManager() {
    for (auto &itr : _bindings) {
        delete itr.second;
        itr.second = nullptr;
    }
}

template <class T>
bool EventManager::addCallback(const std::string& name,
                        const T& func) {
    return _callbacks.emplace(name, func).second;
}

sf::Vector2i EventManager::getMousePos(sf::RenderWindow* wind = nullptr) {
    return (wind ? sf::Mouse::getPosition(*wind) : sf::Mouse::getPosition());
}

void EventManager::removeCallback(const std::string& name) {
    _callbacks.erase(name);
}

bool EventManager::addBinding(Binding *binding) {
    if (_bindings.find(binding->_name) != _bindings.end()) {
        return false;
    }
    return _bindings.emplace(binding->_name, binding).second;
}

bool EventManager::removeBinding(std::string name) {
    auto itr = _bindings.find(name);
    if (itr == _bindings.end()) {
        return false;
    }
    delete itr->second;
    _bindings.erase(itr);
    return true;
}

void EventManager::handleEvent(sf::Event& event) {
    for (auto &b_itr : _bindings) {
        Binding* bind = b_itr.second;
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

void EventManager::update() {
    if (!_hasFocus) {
        return;
    }
    for (auto &b_itr : _bindings) {
        Binding* bind = b_itr.second;
        for (auto &e_itr : bind->_events) {
            switch (e_itr.first) {
                case(EventType::Keyboard) :
                    if (sf::Keyboard::isKeyPressed( sf::Keyboard::Key(e_itr.second._code))) {
                        if (bind->_details._keyCode != -1) {
                            bind->_details._keyCode = e_itr.second._code;
                        }
                            ++(bind->_count);
                    }
                    break;

                case(EventType::Mouse) :
                    if (sf::Mouse::isButtonPressed( sf::Mouse::Button(e_itr.second._code))) {
                        if (bind->_details._keyCode != -1) {
                            bind->_details._keyCode = e_itr.second._code;
                        }
                        ++(bind->_count);
                    }
                    break;
            }
        }

        if (bind->_events.size() == bind->_count) {
            auto callItr = _callbacks.find(bind->_name);
            if(callItr != _callbacks.end()) {
                callItr->second(&bind->_details);
            }
        }
            bind->_count = 0; bind->_details.Clear();
    }
}

void EventManager::loadBindings() {
    std::string delimiter = ":";
    std::ifstream bindings;
    bindings.open("keys.cfg");
        if (!bindings.is_open()) {
            std::cout << "! Failed loading keys.cfg." << std::endl;
            return;
        }

    std::string line;
    while (std::getline(bindings, line)) {
        std::stringstream keystream(line);
        std::string callbackName;
        keystream >> callbackName;
        Binding* bind = new Binding(callbackName);

        while (!keystream.eof()) {
            std::string keyval;
            keystream >> keyval;
            int start = 0;
            int end = keyval.find(delimiter);
            if (end == std::string::npos) {
                delete bind;
                bind = nullptr;
                break;
            }
            EventType type = EventType(stoi(keyval.substr(start, end - start)));
            int code = stoi(keyval.substr(end + delimiter.length(),
            keyval.find(delimiter, end + delimiter.length())));
            EventInfo eventInfo; eventInfo._code = code;
            bind->BindEvent(type, eventInfo);
        }

        if (!addBinding(bind)) {
            delete bind;
        }
        bind = nullptr;
    }
    bindings.close();
}
