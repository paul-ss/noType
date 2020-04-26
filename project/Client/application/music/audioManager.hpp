#pragma once

#include "resourceManager.hpp"
#include "utils.hpp"

#include <SFML/Audio.hpp>

class AudioManager : public ResourceManager<AudioManager, sf::SoundBuffer> {
public:
    AudioManager() : ResourceManager("resources/audio.cfg") {}
    sf::SoundBuffer* Load(const std::string& l_path) {
        sf::SoundBuffer* sound = new sf::SoundBuffer();
        if (!sound->loadFromFile(
            utils::GetWorkingDirectory() + l_path))
        {
            delete sound;
            sound = nullptr;
            std::cerr << "! Failed to load sound: " << l_path << std::endl;
        }
        return sound;
    }
};
