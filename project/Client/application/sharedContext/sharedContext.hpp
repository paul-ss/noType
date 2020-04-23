#pragma once

#include "window.hpp"
#include "eventManager.hpp"

struct SharedContext {
    SharedContext(): _window(nullptr), _eventManager(nullptr) {}
    Window* _window;
    EventManager* _eventManager;
};
