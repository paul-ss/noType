#include <filesystem>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "eventManager.hpp"
#include "exceptions.hpp"
#include "logger.hpp"

#define BINDINGS_FILE_PATH "assets/bindings.json"

EventManager::EventManager(): _currentState(StateType(0)), _hasFocus(true) {
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
    if (_bindings.find(binding->name) != _bindings.end()) {
        return false;
    }
    return _bindings.emplace(binding->name, binding).second;
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
        for (auto &e_itr : bind->events) {
            EventType sfmlEvent = (EventType)l_event.type;
            if (e_itr.first != sfmlEvent) {
                continue;
            }
            if (sfmlEvent == EventType::KeyDown || sfmlEvent == EventType::KeyUp) {
                if (e_itr.second.code == l_event.key.code) {
                    if (bind->details.keyCode != -1) {
                        bind->details.keyCode = e_itr.second.code;
                    }
                    ++(bind->count);
                    break;
                }
            } else if (sfmlEvent == EventType::MButtonDown || sfmlEvent == EventType::MButtonUp) {
                if (e_itr.second.code == l_event.mouseButton.button) {
                    bind->details.mouse.x = l_event.mouseButton.x;
                    bind->details.mouse.y = l_event.mouseButton.y;
                    if (bind->details.keyCode != -1) {
                        bind->details.keyCode = e_itr.second.code;
                    }
                    ++(bind->count);
                    break;
                }
            } else {
                if (sfmlEvent == EventType::MouseWheel) {
                    bind->details.mouseWheelDelta = l_event.mouseWheel.delta;
                } else if (sfmlEvent == EventType::WindowResized) {
                    bind->details.size.x = l_event.size.width;
                    bind->details.size.y = l_event.size.height;
                } else if (sfmlEvent == EventType::TextEntered) {
                    bind->details.textEntered = l_event.text.unicode;
                }
                ++(bind->count);
            }
        }
    }
}

void EventManager::HandleEvent(const GUI_Event& l_event) {
    for (auto &b_itr : _bindings) {
        auto bind = b_itr.second;
        for (auto &e_itr : bind->events) {
            if (e_itr.first != EventType::GUI_Click && e_itr.first != EventType::GUI_Release &&
                e_itr.first != EventType::GUI_Hover && e_itr.first != EventType::GUI_Leave) {
                continue;
            }
            if ((e_itr.first == EventType::GUI_Click && l_event.type != GUI_EventType::Click) ||
                (e_itr.first == EventType::GUI_Release && l_event.type != GUI_EventType::Release) ||
                (e_itr.first == EventType::GUI_Hover && l_event.type != GUI_EventType::Hover) ||
                (e_itr.first == EventType::GUI_Leave && l_event.type != GUI_EventType::Leave)) {
                    continue;
            }
            if ((e_itr.second.gui.interface == l_event.interface) ||
                (e_itr.second.gui.element == l_event.element)) {
                    continue;
            }
            bind->details.guiInterface = l_event.interface;
            bind->details.guiElement = l_event.element;
            ++(bind->count);
        }
    }
}

void EventManager::Update() {
    if (!_hasFocus) {
        return;
    }
    for (auto &b_itr : _bindings) {
        auto bind = (b_itr.second);
        for (auto &e_itr : bind->events) {
            switch (e_itr.first) {
                case(EventType::Keyboard) : {
                    if (sf::Keyboard::isKeyPressed( sf::Keyboard::Key(e_itr.second.code))) {
                        if (bind->details.keyCode != -1) {
                            bind->details.keyCode = e_itr.second.code;
                        }
                        ++(bind->count);
                    }
                    break;
                }

                case(EventType::Mouse) : {
                    if (sf::Mouse::isButtonPressed( sf::Mouse::Button(e_itr.second.code))) {
                        if (bind->details.keyCode != -1) {
                            bind->details.keyCode = e_itr.second.code;
                        }
                        ++(bind->count);
                    }
                    break;
                }

                default: {
                }
            }
        }

        if (bind->events.size() == bind->count) {
            auto stateCallbacks = _callbacks.find(_currentState);
            auto otherCallbacks = _callbacks.find(StateType(0));
            if (stateCallbacks != _callbacks.end()) {
                auto callItr = stateCallbacks->second.find(bind->name);
                if (callItr != stateCallbacks->second.end()) {
                    callItr->second(bind->details);
                }
            }
            if (otherCallbacks != _callbacks.end()) {
                auto callItr = otherCallbacks->second.find(bind->name);
                if (callItr != otherCallbacks->second.end()) {
                    callItr->second(bind->details);
                }
            }
        }

        bind->count = 0;
        bind->details.Clear();
    }
}

void EventManager::loadBindings() {
    try {
        boost::property_tree::ptree root;
        boost::property_tree::read_json(BINDINGS_FILE_PATH, root);

        //for (boost::property_tree::ptree::value_type& keyEvent : root.get_child("events.keys")) {
        //    auto bind = std::make_shared<Binding>(keyEvent.first.data());
//
        //    auto eventType = keyEvent.second.get<EventType>("eventType");
        //    auto keyCode = keyEvent.second.get<EventInfo>("keyCode");
        //    bind->BindEvent(eventType, keyCode);
//
        //    if (!AddBinding(bind)) {
        //        throw InvalidCmd();
        //    }
        //}
//
        //for (boost::property_tree::ptree::value_type& guiEvent : root.get_child("events.gui")) {
        //    auto bind = std::make_shared<Binding>(guiEvent.first.data());
        //    EventInfo eventInfo;
//
        //    auto eventType = guiEvent.second.get<EventType>("eventType");
        //    eventInfo.gui.interface = guiEvent.second.get<std::string>("interface");
        //    eventInfo.gui.element = guiEvent.second.get<std::string>("function");
        //    bind->BindEvent(eventType, eventInfo);
//
        //    if (!AddBinding(bind)) {
        //        throw InvalidCmd();
        //    }
        //}

    } catch (const boost::property_tree::ptree_error& e) {
        BOOST_LOG_TRIVIAL(error) << e.what();

    } catch (const InvalidCmd& e) {
        BOOST_LOG_TRIVIAL(error) << "Failed to add new Binding " << e.what();
    }
}
