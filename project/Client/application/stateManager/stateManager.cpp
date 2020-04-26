#include "stateManager.hpp"

StateManager::StateManager(SharedContext* shared) : _shared(shared) {
    registerState<IntroState>(StateType::Intro);
    registerState<MainMenuState>(StateType::MainMenu);
    //registerState<GameState>(StateType::Game);
    //registerState<PauseState>(StateType::Paused);
}

StateManager::~StateManager(){
    for (auto &itr : _states){
        itr.second->onDestroy();
        delete itr.second;
    }
}

void StateManager::draw() {
    if (_states.empty()) {
        return;
    }

    if (_states.back().second->isTransparent() && _states.size() > 1) {
        auto itr = _states.end();
        while (itr != _states.begin()) {
            if (itr != _states.end()) {
                if (!itr->second->isTransparent()) {
                    break;
                }
            }
            --itr;
        }
        for (; itr != _states.end(); ++itr) {
            itr->second->draw();
        }
    } else {
        _states.back().second->draw();
    }
}

void StateManager::update(const sf::Time& time) {
    if (_states.empty()) {
        return;
    }
    if (_states.back().second->isTranscendent() && _states.size() > 1) {
        auto itr = _states.end();
        while (itr != _states.begin()) {
            if (itr != _states.end()) {
                if (!itr->second->isTranscendent()){
                    break;
                }
            }
            --itr;
        }
        for (; itr != _states.end(); ++itr) {
            itr->second->update(time);
        }
    } else {
        _states.back().second->update(time);
    }
}

SharedContext* StateManager::getContext() {
    return _shared;
}

bool StateManager::hasState(const StateType& type) {
    for (auto itr = _states.begin(); itr != _states.end(); ++itr) {
        if (itr->first == type) {
            auto removed = std::find(_toRemove.begin(), _toRemove.end(), type);
            if (removed == _toRemove.end()) {
                return true;
            }
            return false;
        }
    }
    return false;
}

void StateManager::remove(const StateType& type) {
    _toRemove.push_back(type);
}

void StateManager::processRequests() {
    while (_toRemove.begin() != _toRemove.end()) {
        removeState(*_toRemove.begin());
        _toRemove.erase(_toRemove.begin());
    }
}

void StateManager::switchTo(const StateType& type) {
    _shared->_eventManager->setCurrentState(type);
    for (auto itr = _states.begin(); itr != _states.end(); ++itr) {
        if (itr->first == type) {
            _states.back().second->deactivate();
            StateType tmp_type = itr->first;
            auto tmp_state = itr->second;
            _states.erase(itr);
            _states.emplace_back(tmp_type, tmp_state);
            tmp_state->activate();
            return;
        }
    }
    if (!_states.empty()) {
        _states.back().second->deactivate();
    }
    createState(type);
    _states.back().second->activate();
}

void StateManager::createState(const StateType& type) {
    auto newState = _stateFactory.find(type);
    if (newState == _stateFactory.end()) {
        return;
    }

    auto state = newState->second();
    _states.emplace_back(type, state);
    state->onCreate();
}

void StateManager::removeState(const StateType& l_type){
    for (auto itr = _states.begin();
        itr != _states.end(); ++itr)
    {
        if (itr->first == l_type){
            itr->second->onDestroy();
            delete itr->second;
            _states.erase(itr);
            return;
        }
    }
}
