#include "beforeGameState.hpp"

BeforeGameState::BeforeGameState(std::weak_ptr<StateManager> l_stateManager)
    : BaseState(l_stateManager) {}

BeforeGameState::~BeforeGameState() {}

    void BeforeGameState::OnCreate() {}
    void BeforeGameState::OnDestroy() {}

    void BeforeGameState::Activate() {}
    void BeforeGameState::Deactivate() {}

    void BeforeGameState::Update(const sf::Time& l_time) {}
    void BeforeGameState::Draw() {}
    