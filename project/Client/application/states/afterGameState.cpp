#include "afterGameState.hpp"
#include "sharedContext.hpp"
#include "logger.hpp"

AfterGameState::AfterGameState(std::weak_ptr<SharedContext> l_context)
    : BaseState(l_context) {}

    void AfterGameState::OnCreate() {}
    void AfterGameState::OnDestroy() {}

    void AfterGameState::Activate() {}
    void AfterGameState::Deactivate() {}

    void AfterGameState::Update(const sf::Time& l_time) {}
    void AfterGameState::Draw() {}
