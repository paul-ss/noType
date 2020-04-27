#pragma once

#include "window.hpp"
#include "eventManager.hpp"
#include "fontManager.hpp"
#include "textureManager.hpp"
#include "audioManager.hpp"
#include "musicManager.hpp"
#include "GUI_manager.hpp"

struct SharedContext {
    SharedContext():
            _window(nullptr),
            _eventManager(nullptr),
            _audioManager(nullptr),
            _soundManager(nullptr),
            _textureManager(nullptr),
            _fontManager(nullptr),
            _guiManager(nullptr) {}

    Window* _window;
    EventManager* _eventManager;
    AudioManager* _audioManager;
    SoundManager* _soundManager;
    TextureManager* _textureManager;
    FontManager* _fontManager;
    GUI_Manager* _guiManager;
};
