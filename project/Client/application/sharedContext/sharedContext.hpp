#pragma once

#include <memory>

#include "window.hpp"
#include "eventManager.hpp"
#include "GUI_manager.hpp"
#include "fontManager.hpp"
#include "textureManager.hpp"
#include "audioManager.hpp"
#include "soundManager.hpp"

struct SharedContext {
    std::weak_ptr<Window> _window;
    std::weak_ptr<EventManager> _eventManager;
    std::weak_ptr<AudioManager> _audioManager;
    std::weak_ptr<SoundManager> _soundManager;
    std::weak_ptr<TextureManager> _textureManager;
    std::weak_ptr<FontManager> _fontManager;
    std::weak_ptr<GUI_Manager> _guiManager;
};
