#pragma once

#include "window.hpp"
#include "eventManager.hpp"

struct SharedContext {
    SharedContext(): _window(nullptr), _eventManager(nullptr) {}
    std::shared_ptr<Window> _window;
    std::shared_ptr<EventManager> _eventManager;
};
