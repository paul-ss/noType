#include "window.hpp"
#include "eventManager.hpp"

struct SharedContext {
    SharedContext(): _wind(nullptr), _eventManager(nullptr) {}
    Window* _wind;
    EventManager* _eventManager;
};
