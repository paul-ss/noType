#pragma once

#include <filesystem>

#include <SFML/Audio.hpp>

#include "resourceManager.hpp"

#define AUDIO_FILE_PATH "assets/audio.cfg"

class AudioManager : public ResourceManager<AudioManager, sf::SoundBuffer> {
public:
    AudioManager() : ResourceManager(AUDIO_FILE_PATH) {}
    sf::SoundBuffer* load(const std::string& l_path) {
        sf::SoundBuffer* sound = new sf::SoundBuffer();
        if (!sound->loadFromFile(std::filesystem::absolute(l_path))) {
            delete sound;
            sound = nullptr;
            BOOST_LOG_TRIVIAL(error) << "Failed to load audio file: " << l_path;
        }
        return sound;
    }
};
