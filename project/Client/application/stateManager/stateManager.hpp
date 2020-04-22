#pragma once

#include "baseState.hpp"
#include "sharedContext.hpp"
#include "introState.hpp"

#include <functional>
#include <unordered_map>

enum class StateType{
Intro = 1, MainMenu, Game, Paused, GameOver, Credits
};

using StateContainer = 
        std::vector< std::pair<StateType, std::shared_ptr<BaseState> > >;

using TypeContainer = std::vector<StateType>;

using StateFactory = std::unordered_map< StateType,
        std::function<std::shared_ptr<BaseState>(void)> >;

class StateManager{
    public:
        StateManager(SharedContext& shared);
        ~StateManager();

        void update(const sf::Time& time);
        void draw();

        void processRequests();

        SharedContext& getContext();
        bool hasState(const StateType& type);

        void switchTo(const StateType& type);
        void remove(const StateType& type);

    private:
        void createState(const StateType& type);
        void removeState(const StateType& l_type);
        template<class T>
        void registerState(const StateType& type);

    private:
        SharedContext& _shared;
        StateContainer _states;
        TypeContainer _toRemove;
        StateFactory _stateFactory;
};
