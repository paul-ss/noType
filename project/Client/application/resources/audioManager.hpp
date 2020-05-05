#pragma once

#include <SFML/Audio.hpp>

#include "resourceManager.hpp"

class AudioManager : public ResourceManager<AudioManager, sf::SoundBuffer> {
public:
    AudioManager() : ResourceManager("assets/audio.cfg") {}
    sf::SoundBuffer* load(const std::string& path) {
        sf::SoundBuffer* sound = new sf::SoundBuffer();
        if (!sound->loadFromFile(utils::GetWorkingDirectory() + path)) {
            delete sound;
            sound = nullptr;
            std::cerr << "! Failed to load sound: " << path << std::endl;
        }
        return sound;
    }
};
