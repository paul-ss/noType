#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

class StateManager;

class BaseState {
    friend class StateManager;

public:
    explicit BaseState(StateManager* l_stateManager) :
            _stateMgr(l_stateManager),
            _transparent(false),
            _transcendent(false) {}

    virtual ~BaseState() = default;

    virtual void OnCreate() = 0;
    virtual void OnDestroy() = 0;

    virtual void Activate() = 0;
    virtual void Deactivate() = 0;

    virtual void Update(const sf::Time& l_time) = 0;
    virtual void Draw() = 0;

    void SetTransparent(const bool& l_transparent) {
        _transparent = l_transparent;
    }

    bool IsTransparent()const {
        return _transparent;
    }

    void SetTranscendent(const bool& l_transcendence) {
    _transcendent = l_transcendence;
    }

    bool IsTranscendent() const {
        return _transcendent;
    }

    StateManager* GetStateManager() {
        return _stateMgr;
    }

protected:
    StateManager* _stateMgr;
    bool _transparent;
    bool _transcendent;
};
