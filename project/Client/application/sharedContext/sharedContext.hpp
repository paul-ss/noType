#pragma once

#include "window.hpp"
#include "eventManager.hpp"
//#include "fontManager.hpp"
#include "audioManager.hpp"
#include "soundManager.hpp"
//#include "GUI_manager.hpp"

struct SharedContext {
    SharedContext():
            _window(nullptr),
            _eventManager(nullptr),
            _audioManager(nullptr),
            _soundManager(nullptr) {}

    Window* _window;
    EventManager* _eventManager;
    AudioManager* _audioManager;
    SoundManager* _soundManager;
};
