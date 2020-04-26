#include "soundManager.hpp"

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

bool SoundManager::StopMusic(const StateType& state){
    auto music = _music.find(_currentState);
    if (music == _music.end()){
        return false;
    }
    if (!music->second.second){
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
    if (!music->second.second){
        return false;
    }
    music->second.second->pause();
    music->second.first._manualPaused = true;
    return true;
}

bool SoundManager::PlayMusic(const std::string& musicId, float volume, bool loop){
    auto s = _music.find(_currentState);
    if (s == _music.end()){ return false; }
    std::string path = _audioManager->getPath(musicId);
    if (path == ""){ return false; }
    if (!s->second.second){
        s->second.second = new sf::Music();
        ++_numSounds;
    }
    sf::Music* music = s->second.second;
    if (!music->openFromFile(utils::getWorkingDirectory() + path)){
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
