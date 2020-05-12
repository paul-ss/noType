#pragma once

#include <filesystem>

#include <SFML/Audio.hpp>

#include "resourceManager.hpp"

#define AUDIO_FILE_PATH "assets/audio.json"

class AudioManager : public ResourceManager<AudioManager, sf::SoundBuffer> {
public:
    AudioManager() : ResourceManager(AUDIO_FILE_PATH, "audio") {}

    std::shared_ptr<sf::SoundBuffer> load(const std::string& l_path) {
        auto sound = std::make_shared<sf::SoundBuffer>();
        if (!sound->loadFromFile(std::filesystem::absolute(l_path))) {
            sound.reset();
            sound = nullptr;
            BOOST_LOG_TRIVIAL(error) << "Failed to load audio file: " << l_path;
        }
        return sound;
    }
};
