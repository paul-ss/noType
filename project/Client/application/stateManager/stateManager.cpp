#include "stateManager.hpp"

StateManager::StateManager(std::weak_ptr<SharedContext> l_sharedContext) : _shared(l_sharedContext) {
    registerState<IntroState>(StateType::Intro);
    registerState<MainMenuState>(StateType::MainMenu);
}

StateManager::~StateManager() {
    for (auto &itr : _states) {
        itr.second->OnDestroy();
        itr.second.reset();
    }
}

void StateManager::Draw() {
    if (_states.empty()) {
        return;
    }

    if (_states.back().second->IsTransparent() && _states.size() > 1) {
        auto itr = _states.end();
        while (itr != _states.begin()) {
            if (itr != _states.end()) {
                if (!itr->second->IsTransparent()) {
                    break;
                }
            }
            --itr;
        }
        for (; itr != _states.end(); ++itr) {
            itr->second->Draw();
        }
    } else {
        _states.back().second->Draw();
    }
}

void StateManager::Update(const sf::Time& time) {
    if (_states.empty()) {
        return;
    }
    if (_states.back().second->IsTranscendent() && _states.size() > 1) {
        auto itr = _states.end();
        while (itr != _states.begin()) {
            if (itr != _states.end()) {
                if (!itr->second->IsTranscendent()){
                    break;
                }
            }
            --itr;
        }
        for (; itr != _states.end(); ++itr) {
            itr->second->Update(time);
        }
    } else {
        _states.back().second->Update(time);
    }
}

std::weak_ptr<SharedContext> StateManager::GetContext() {
    return _shared;
}

bool StateManager::HasState(const StateType& type) {
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

void StateManager::Remove(const StateType& type) {
    _toRemove.push_back(type);
}

void StateManager::ProcessRequests() {
    while (_toRemove.begin() != _toRemove.end()) {
        removeState(_toRemove.front());
        _toRemove.erase(_toRemove.begin());
    }
}

void StateManager::SwitchTo(const StateType& type) {
    _shared.lock()->eventManager.lock()->SetCurrentState(type);
    _shared.lock()->soundManager.lock()->ChangeState(type);
    for (auto itr = _states.begin();
        itr != _states.end(); ++itr) {
        if (itr->first == type) {
            _states.back().second->Deactivate();
            StateType tmp_type = itr->first;
            auto tmp_state = itr->second;
            _states.erase(itr);
            _states.emplace_back(tmp_type, tmp_state);
            tmp_state->Activate();
            return;
        }
    }

    // State with type wasn't found.
    if (!_states.empty()) {
        _states.back().second->Deactivate();
    }
    createState(type);
    _states.back().second->Activate();
}

void StateManager::createState(const StateType& type) {
    auto newState = _stateFactory.find(type);
    if (newState == _stateFactory.end()) {
        BOOST_LOG_TRIVIAL(error) << "Not register state";
        return;
    }

    auto state = newState->second();
    _states.emplace_back(type, state);
    state->OnCreate();
}

void StateManager::removeState(const StateType& type) {
    for (auto itr = _states.begin();
        itr != _states.end(); ++itr) {
        if (itr->first == type){
            itr->second->OnDestroy();
            itr->second.reset();
            _states.erase(itr);
            _shared.lock()->soundManager.lock()->RemoveState(type);
            return;
        }
    }
}
