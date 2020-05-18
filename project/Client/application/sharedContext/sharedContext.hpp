#pragma once

#include <memory>

#include "window.hpp"
#include "eventManager.hpp"
#include "fontManager.hpp"
#include "textureManager.hpp"
#include "audioManager.hpp"
#include "soundManager.hpp"

struct SharedContext {
    std::weak_ptr<Window> window;
    std::weak_ptr<EventManager> eventManager;
    std::weak_ptr<AudioManager> audioManager;
    std::weak_ptr<SoundManager> soundManager;
    std::weak_ptr<TextureManager> textureManager;
    std::weak_ptr<FontManager> fontManager;
};
