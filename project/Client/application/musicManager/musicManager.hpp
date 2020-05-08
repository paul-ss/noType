#pragma once

#include "audioManager.hpp"
#include "utils.hpp"

#include <unordered_map>
#include <vector>
#include <string>

#include <SFML/Audio.hpp>

using SoundID = int;

enum class StateType;

struct SoundInfo{
    SoundInfo(const std::string& name): _name(name), _manualPaused(false) {}
    std::string _name;
    bool _manualPaused;
};

using MusicContainer = std::unordered_map<StateType, std::pair<SoundInfo, sf::Music*>>;

class SoundManager {
    public:
        SoundManager(AudioManager* audioMgr) : _audioManager{audioMgr} {};
        ~SoundManager() = default;

        void ChangeState(const StateType& state);
        void RemoveState(const StateType& state);
        void Update(float l_dT);
        bool PlayMusic(const std::string& musicId, float volume = 100.f, bool loop = false);
        bool PlayMusic(const StateType& state);
        bool StopMusic(const StateType& state);
        bool PauseMusic(const StateType& state);

        static const int Max_Sounds = 150;
        static const int Sound_Cache = 75;

    private:
        void pauseAll(const StateType& state);
        void unPauseAll(const StateType& state);

    private:
        MusicContainer _music;
        StateType _currentState;
        unsigned int _numSounds;
        float _elapsed;
        AudioManager* _audioManager;
};
