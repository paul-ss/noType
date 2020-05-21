#pragma once

#include <memory>
#include <vector>

#include "Network.hpp"

#include "window.hpp"
#include "eventManager.hpp"
#include "audioManager.hpp"
#include "soundManager.hpp"
#include "textureManager.hpp"
#include "fontManager.hpp"
#include "stateManager.hpp"
#include "baseState.hpp"
#include "baseElement.hpp"

struct SharedContext {
    std::weak_ptr<Window> window;
    std::weak_ptr<EventManager> eventManager;
    std::weak_ptr<AudioManager> audioManager;
    std::weak_ptr<SoundManager> soundManager;
    std::weak_ptr<TextureManager> textureManager;
    std::weak_ptr<FontManager> fontManager;
    std::weak_ptr<StateManager> stateManager;

    //To share common ui elements between states
    std::unordered_map<ElementName, std::shared_ptr<BaseElement>> sharedElements;

    std::string uuid;
    std::weak_ptr<Network::INetworkManager> networkManager;
    std::weak_ptr<Network::Client::IQueueManager> queueManager;
};
