#pragma once

#include <filesystem>

#include <SFML/Audio.hpp>

#include "resourceManager.hpp"

class AudioManager : public ResourceManager<AudioManager, sf::SoundBuffer> {
public:
    AudioManager() : ResourceManager("assets/audio.cfg") {}
    sf::SoundBuffer* load(const std::string& path) {
        sf::SoundBuffer* sound = new sf::SoundBuffer();
        if (!sound->loadFromFile(std::filesystem::absolute(path))) {
            delete sound;
            sound = nullptr;
            BOOST_LOG_TRIVIAL(error) << "Failed to load audio file: " << path;
        }
        return sound;
    }
};
