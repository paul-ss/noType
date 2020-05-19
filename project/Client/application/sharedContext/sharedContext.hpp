#pragma once

#include <memory>

#include "Network.hpp"

#include "window.hpp"
#include "eventManager.hpp"
#include "fontManager.hpp"
#include "textureManager.hpp"
#include "audioManager.hpp"
#include "soundManager.hpp"
#include "baseElement.hpp"

class BaseElement;

struct SharedContext {
    std::weak_ptr<Window> window;
    std::weak_ptr<EventManager> eventManager;
    std::weak_ptr<AudioManager> audioManager;
    std::weak_ptr<SoundManager> soundManager;
    std::weak_ptr<TextureManager> textureManager;
    std::weak_ptr<FontManager> fontManager;

    //share common ui elements between states
    std::unordered_map<std::string, std::shared_ptr<BaseElement>> elementContainer;

    std::string uuid;
    std::weak_ptr<Network::INetworkManager> networkManager;
    std::weak_ptr<Network::Client::IQueueManager> queueManager;
};
