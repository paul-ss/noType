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
    auto randNum = std::rand() % numOfTracks;
    std::string name = _musicNames[randNum];
    _musicNames.erase(std::remove(_musicNames.begin(), _musicNames.end(), _musicNames[randNum]),
                _musicNames.end());
    return name;
}
