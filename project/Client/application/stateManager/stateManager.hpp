#pragma once

#include <functional>
#include <unordered_map>

#include "gameState.hpp"
#include "introState.hpp"
#include "mainMenuState.hpp"
#include "beforeGameState.hpp"
#include "afterGameState.hpp"

struct SharedContext;

enum class StateType{
    Intro = 1,
    MainMenu,
    BeforeGame,
    Game,
    AfterGame
};

using StateContainer =
        std::vector<std::pair<StateType, std::shared_ptr<BaseState>>>;

using TypeContainer = std::vector<StateType>;

using StateFactory = std::unordered_map< StateType,
        std::function<std::shared_ptr<BaseState>(void)>>;

class StateManager {
public:
    explicit StateManager(std::weak_ptr<SharedContext> shared);
    ~StateManager();

    void Update(const sf::Time& time);
    void Draw();

    void ProcessRequests();

    std::weak_ptr<SharedContext> GetContext();
    bool HasState(const StateType& type);

    void SwitchTo(const StateType& type);
    void Remove(const StateType& type);

private:
    void createState(const StateType& type);
    void removeState(const StateType& l_type);

    template<class T>
    void registerState(const StateType& type) {
        _stateFactory[type] = [&]()->std::shared_ptr<BaseState> {
            return std::make_shared<T>(_shared);
        };
    }

private:
    std::weak_ptr<SharedContext> _shared;
    StateContainer _states;
    TypeContainer _toRemove;
    StateFactory _stateFactory;
};
