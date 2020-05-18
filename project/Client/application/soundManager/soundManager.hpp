#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>

#include <SFML/Audio.hpp>

//using SoundID = int;

//enum class StateType;

//struct SoundInfo {
//    SoundInfo() = default;
//    explicit SoundInfo(const std::string& name): _name(name), _manualPaused(false) {}
//    std::string _name;
//    bool _manualPaused;
//};
//
//using MusicContainer = std::unordered_map<StateType, std::pair<SoundInfo, std::shared_ptr<sf::Music>>>;
struct SharedContext;
class SoundManager {
public:
    explicit SoundManager(std::weak_ptr<SharedContext> l_sharedContext);
    ~SoundManager() = default;

    //void ChangeState(const StateType& l_state);
    //void RemoveState(const StateType& l_state);
    //void Update(float l_dT);
    //bool PlayMusic(const std::string& l_musicId, float l_volume = 100.f, bool l_loop = false);
    bool PlayMusic();
    //bool StopMusic(const StateType& l_state);
    bool PauseMusic(const StateType& l_state);

    //static const int Max_Sounds = 150;
    //static const int Sound_Cache = 75;

private:
    //void pauseAll(const StateType& l_state);
    //void unPauseAll(const StateType& l_state);

private:
    //MusicContainer _musicContainer;
    //StateType _currentState;
    //unsigned int _numSounds;
    //float _elapsed;
    std::vector<std::string&> _musicNames;
    std::shared_ptr<sf::Music> _track;
    bool _isPaused;
    std::weak_ptr<SharedContext> _sharedContext;
};
