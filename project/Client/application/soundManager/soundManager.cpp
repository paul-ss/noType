#include <filesystem>

#include <boost/log/trivial.hpp>

#include "soundManager.hpp"
#include "utils.hpp"


void SoundManager::RemoveState(const StateType& l_state) {
    auto music = _musicContainer.find(l_state);
    if (music == _musicContainer.end()) {
        return;
    }

    if (music->second.second){
        --_numSounds;
    }
    _musicContainer.erase(l_state);
}

void SoundManager::pauseAll(const StateType& l_state) {
    auto music = _musicContainer.find(l_state);
    if (music == _musicContainer.end()) {
        return;
    }
    if (!music->second.second) {
        return;
    }
    music->second.second->pause();
}

void SoundManager::unPauseAll(const StateType& l_state) {
    auto music = _musicContainer.find(l_state);
    if (music == _musicContainer.end()) {
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
    
    if (_musicContainer.find(_currentState) != _musicContainer.end()) {
        return;
    }
    _musicContainer.emplace(_currentState, std::make_pair<>(SoundInfo(), std::shared_ptr<sf::Music>()));
}

bool SoundManager::PlayMusic(const StateType& state){
    auto music = _musicContainer.find(_currentState);
    if (music == _musicContainer.end()) {
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
    auto music = _musicContainer.find(_currentState);
    if (music == _musicContainer.end()) {
        return false;
    }
    if (!music->second.second) {
        return false;
    }
    music->second.second->stop();
    music->second.second.reset();
    music->second.second = nullptr;
    --_numSounds;
    return true;
}

bool SoundManager::PauseMusic(const StateType& state) {
    auto music = _musicContainer.find(_currentState);
    if (music == _musicContainer.end()) {
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
    auto currStateMusic = _musicContainer.find(_currentState);
    if (currStateMusic == _musicContainer.end()) {
        BOOST_LOG_TRIVIAL(error) << "Failed to play music: " << musicId;
        return false;
    }

    auto audioManager = _audioManager.lock();
    if (!audioManager) {
        BOOST_LOG_TRIVIAL(error) << "AudioManager is expired";
        return false;
    }
    std::string path = audioManager->GetPath(musicId);
    if (path.empty()) {
        BOOST_LOG_TRIVIAL(error) << "Wrong path to music file: " << path;
        return false;
    }

    if (!currStateMusic->second.second) {
        currStateMusic->second.second = std::make_shared<sf::Music>();
        ++_numSounds;
    }

    auto&& music = currStateMusic->second.second;
    if (!music->openFromFile(std::filesystem::absolute(path))) {
        music.reset();
        --_numSounds;
        music = nullptr;
        BOOST_LOG_TRIVIAL(error) << "Failed to load music from file: " << musicId;
        return false;
    }

    music->setLoop(loop);
    music->setVolume(volume);
    music->setRelativeToListener(true);
    music->play();
    currStateMusic->second.first._name = musicId;
    return true;
}

void SoundManager::Update(float dT) {
    _elapsed += dT;
    if (_elapsed < 0.33f) {
        return;
    }
    // Run once every third of a second.

    _elapsed = 0;

    auto music = _musicContainer.find(_currentState);
    if (music == _musicContainer.end()) {
        return;
    }
    if (!music->second.second) {
        return;
    }
    if (music->second.second->getStatus()) {
        return;
    }
    music->second.second.reset();
    music->second.second = nullptr;
    --_numSounds;
}
