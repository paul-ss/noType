#include "baseState.hpp"
#include "sharedContext.hpp"
#include "exceptions.hpp"
#include "logger.hpp"

BaseState::BaseState(std::weak_ptr<SharedContext> l_context) :
        _transparent(false),
        _transcendent(false),
        _context(l_context) {}

void BaseState::SetTransparent(const bool& l_transparent) {
    _transparent = l_transparent;
}

bool BaseState::IsTransparent() const {
    return _transparent;
}

void BaseState::SetTranscendent(const bool& l_transcendence) {
    _transcendent = l_transcendence;
}

bool BaseState::IsTranscendent() const {
    return _transcendent;
}

std::shared_ptr<SharedContext> BaseState::GetSharedContext() {
    return std::shared_ptr<SharedContext> { _context };
}

std::shared_ptr<StateManager> BaseState::GetStateManager() {
    std::shared_ptr<SharedContext> context(_context);
    return std::shared_ptr<StateManager> { context->stateManager };
}

std::shared_ptr<EventManager> BaseState::GetEventManager() {
    std::shared_ptr<SharedContext> context(_context);
    return std::shared_ptr<EventManager> { context->eventManager };
}

std::shared_ptr<Window> BaseState::GetWindow() {
    std::shared_ptr<SharedContext> context(_context);
    return std::shared_ptr<Window> { context->window };
}

std::shared_ptr<sf::RenderWindow> BaseState::GetRenderWindow() {
    std::shared_ptr<SharedContext> context(_context);
    std::shared_ptr<Window> window(context->window);
    return std::shared_ptr<sf::RenderWindow> { window->GetRenderWindow() };
}

std::shared_ptr<Network::Client::IQueueManager> BaseState::GetQueueManager() {
    std::shared_ptr<SharedContext> context(_context);
    return std::shared_ptr<Network::Client::IQueueManager> { context->queueManager };
}

std::shared_ptr<Network::INetworkManager> BaseState::GetNetworkManager() {
    std::shared_ptr<SharedContext> context(_context);
    return std::shared_ptr<Network::INetworkManager> { context->networkManager };
}

void BaseState::checkNetStatus(const Network::Status l_status, const std::string& l_error) {
    if (l_status == Network::Status::Fail) {
        throw InvalidResponse(l_error);
    }
}

void BaseState::drawElement(ElementName l_name) {
    auto intro = _elements.find(l_name);
    if (intro != _elements.end()) {
        intro->second->Draw();
    }
}
