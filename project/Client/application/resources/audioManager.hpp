#pragma once

#include "resourceManager.hpp"
#include "utils.hpp"

#include <SFML/Audio.hpp>

class AudioManager : public ResourceManager<AudioManager, sf::SoundBuffer> {
public:
    AudioManager() : ResourceManager("resources/audio.cfg") {}
    sf::SoundBuffer* Load(const std::string& path) {
        sf::SoundBuffer* sound = new sf::SoundBuffer();
        if (!sound->loadFromFile(utils::GetWorkingDirectory() + path)) {
            delete sound;
            sound = nullptr;
            std::cerr << "! Failed to load sound: " << path << std::endl;
        }
        return sound;
    }
};
