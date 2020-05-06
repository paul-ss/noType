#include <filesystem>

#include <boost/log/trivial.hpp>

#include "soundManager.hpp"
#include "utils.hpp"


void SoundManager::RemoveState(const StateType& state) {
    auto music = _music.find(state);
    if (music == _music.end()) {
        return;
    }

    if (music->second.second){
        delete music->second.second;
        --_numSounds;
    }
    _music.erase(state);
}

void SoundManager::pauseAll(const StateType& state) {
    auto music = _music.find(state);
    if (music == _music.end()) {
        return;
    }
    if (!music->second.second) {
        return;
    }
    music->second.second->pause();
}

void SoundManager::unPauseAll(const StateType& state) {
    auto music = _music.find(state);
    if (music == _music.end()) {
        return;
    }

    if (!music->second.second || music->second.first._manualPaused) {
        return;
    }
    music->second.second->play();
}

void SoundManager::ChangeState(const StateType& state) {
    pauseAll(_currentState);
    unPauseAll(state);
    _currentState = state;
    
    if (_music.find(_currentState) != _music.end()) {
        return;
    }
    SoundInfo info("");
    sf::Music* music = nullptr;
    _music.emplace(_currentState, std::make_pair(info, music));
}

bool SoundManager::PlayMusic(const StateType& state){
    auto music = _music.find(_currentState);
    if (music == _music.end()) {
        return false;
    }
    if (!music->second.second){
        return false;
    }
    music->second.second->play();
    music->second.first._manualPaused = false;
    return true;
}

bool SoundManager::StopMusic(const StateType& state) {
    auto music = _music.find(_currentState);
    if (music == _music.end()) {
        return false;
    }
    if (!music->second.second) {
        return false;
    }
    music->second.second->stop();
    delete music->second.second;
    music->second.second = nullptr;
    --_numSounds;
    return true;
}

bool SoundManager::PauseMusic(const StateType& state) {
    auto music = _music.find(_currentState);
    if (music == _music.end()) {
        return false;
    }
    if (!music->second.second) {
        return false;
    }
    music->second.second->pause();
    music->second.first._manualPaused = true;
    return true;
}

bool SoundManager::PlayMusic(const std::string& musicId, float volume, bool loop) {
    auto s = _music.find(_currentState);
    if (s == _music.end()) {
        return false;
    }

    std::string path = _audioManager->GetPath(musicId);
    if (path == "") {
        return false;
    }

    if (!s->second.second) {
        s->second.second = new sf::Music();
        ++_numSounds;
    }

    sf::Music* music = s->second.second;
    if (!music->openFromFile(std::filesystem::absolute(path))) {
        delete music;
        --_numSounds;
        s->second.second = nullptr;
        std::cerr << "[SoundManager] Failed to load music from file: " << musicId << std::endl;
        return false;
    }

    music->setLoop(loop);
    music->setVolume(volume);
    music->setRelativeToListener(true);
    music->play();
    s->second.first._name = musicId;
    return true;
}

void SoundManager::Update(float dT) {
    _elapsed += dT;
    if (_elapsed < 0.33f) {
        return;
    }
    // Run once every third of a second.

    _elapsed = 0;

    auto music = _music.find(_currentState);
    if (music == _music.end()) {
        return;
    }
    if (!music->second.second) {
        return;
    }
    if (music->second.second->getStatus()) {
        return;
    }
    delete music->second.second;
    music->second.second = nullptr;
    --_numSounds;
}
