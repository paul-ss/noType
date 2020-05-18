#include <filesystem>

#include <boost/log/trivial.hpp>

#include "soundManager.hpp"
#include "sharedContext.hpp"
#include "logger.hpp"

SoundManager::SoundManager(std::weak_ptr<SharedContext> l_sharedContext, float l_volume) :
        _volume(l_volume),
        _status(sf::SoundSource::Stopped),
        _sharedContext(l_sharedContext) {

    PlayMusic(getRandTrack());
}

void SoundManager::Mute() {
    if (_status == sf::SoundSource::Paused) {
        _track->play();
        _status = sf::SoundSource::Playing;
    } else {
        _track->pause();
        _status = sf::SoundSource::Paused;
    }
}

bool SoundManager::PlayMusic(const std::string& l_musicId) {
    try {
        std::shared_ptr<SharedContext> sharedContext(_sharedContext);
        std::shared_ptr<AudioManager> audioManager(sharedContext->audioManager);

        audioManager->RequireResource(l_musicId);
        _track = audioManager->GetResource(l_musicId).lock();
        if (!_track) {
            BOOST_LOG_TRIVIAL(error) << "Failed to require resource : " << l_musicId;
            return false;
        }

        auto&& music = _track;
        music->setVolume(_volume);
        music->setRelativeToListener(true);
        music->play();
        return true;

    } catch (const std::bad_weak_ptr &e) {
        BOOST_LOG_TRIVIAL(error) << "AudioManager is expired";
        return false;
    }
}

void SoundManager::Update(const sf::Time& l_dT) {
    _elapsed += l_dT.asSeconds();
    if (_elapsed < 0.33f) {
        return;
    }
    // Run once every third of a second.

    _elapsed = 0;

    if (!_track) {
        return;
    }
    if (_track->getStatus() == sf::SoundSource::Stopped &&
        _track->getStatus() != sf::SoundSource::Paused) {
        PlayMusic(getRandTrack());
    }
}

std::string SoundManager::getRandTrack() {
    if (_musicNames.empty()) {
        try {
            std::shared_ptr<SharedContext>sharedContext(_sharedContext);
            std::shared_ptr<AudioManager>audioManager(sharedContext->audioManager);
            _musicNames = audioManager->getNames();
        } catch (std::bad_weak_ptr& e) {
            BOOST_LOG_TRIVIAL(error) << "[SoundManager getrandtrack] " << e.what();
        } catch (std::length_error& le) {
            std::cout << le.what();
        }
    }
    auto numOfTracks = _musicNames.size();
    std::srand(std::time(nullptr));
    auto randNum = std::rand() % numOfTracks + 1;
    std::string name = _musicNames[randNum];
    _musicNames.erase(std::remove(_musicNames.begin(), _musicNames.end(), _musicNames[randNum]),
                _musicNames.end());
    return name;
}

/*
SoundManager::SoundManager(std::weak_ptr<SharedContext> l_sharedContext) :
            _numSounds(0),
            _elapsed(0.f),
            _sharedContext(l_sharedContext) {};

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

bool SoundManager::PlayMusic(){
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

    try {
        std::shared_ptr<SharedContext> sharedContext(_sharedContext);
        std::shared_ptr<AudioManager> audioManager(sharedContext->audioManager);

        if (!currStateMusic->second.second) {
            audioManager->RequireResource(musicId);
            currStateMusic->second.second = audioManager->GetResource(musicId).lock();
            if (!currStateMusic->second.second) {
                BOOST_LOG_TRIVIAL(error) << "Failed to require resource : " << musicId;
                return false;
            }
            ++_numSounds;
        }

        auto&& music = currStateMusic->second.second;
        music->setLoop(loop);
        music->setVolume(volume);
        music->setRelativeToListener(true);
        music->play();
        currStateMusic->second.first._name = musicId;
        return true;

    } catch (const std::bad_weak_ptr &e) {
        BOOST_LOG_TRIVIAL(error) << "AudioManager is expired";
        return false;
    }
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
}*/
