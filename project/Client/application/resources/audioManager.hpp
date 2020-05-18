#pragma once

#include <filesystem>

#include <SFML/Audio.hpp>

#include "resourceManager.hpp"

#define AUDIO_FILE_PATH "assets/audio.json"

class AudioManager : public ResourceManager<AudioManager, sf::Music> {
public:
    AudioManager() : ResourceManager(AUDIO_FILE_PATH, "audio") {}

    std::vector<std::string> getNames() {
        std::vector<std::string> allTracks;
        for (auto& music : _paths) {
            allTracks.push_back(music.first);
        }
        return allTracks;
    }

    std::shared_ptr<sf::Music> load(const std::string& l_path) {
        auto sound = std::make_shared<sf::Music>();
        if (!sound->openFromFile(std::filesystem::absolute(l_path))) {
            sound.reset();
            sound = nullptr;
            BOOST_LOG_TRIVIAL(error) << "Failed to load audio file: " << l_path;
        }
        return sound;
    }
};
