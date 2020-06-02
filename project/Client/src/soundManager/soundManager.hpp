#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>

#include <SFML/Audio.hpp>

struct SharedContext;

class SoundManager {
public:
    explicit SoundManager(std::weak_ptr<SharedContext> l_sharedContext, float l_volume = 50.0f);
    ~SoundManager() = default;

    void Update(const sf::Time& l_dT);
    bool PlayMusic(const std::string& l_musicId);
    void Mute();

private:
    std::string getRandTrack();

private:
    float _elapsed;
    float _volume;
    sf::SoundSource::Status _status;
    std::vector<std::string> _musicNames;
    std::shared_ptr<sf::Music> _track;
    std::weak_ptr<SharedContext> _sharedContext;
};
