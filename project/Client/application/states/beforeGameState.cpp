#include "beforeGameState.hpp"

BeforeGameState::BeforeGameState(std::weak_ptr<StateManager> l_stateManager)
    : BaseState(l_stateManager) {}

BeforeGameState::~BeforeGameState() {}

void BeforeGameState::OnCreate() {
    try {
        std::shared_ptr<StateManager>stateMgr(_stateMgr);
        std::shared_ptr<SharedContext>context(stateMgr->GetContext());
        std::shared_ptr<Network::NetworkManager>networkManager(context->networkManager);
        std::shared_ptr<Connector::Client::IQueueManager>queueManager(context->queueManager);

        networkManager->Connect();
        Network::InitRequest initRequest;
        std::any data = initRequest;
        auto sendMsg = std::make_unique<Network::Message>
                (Network::MessageType::InitRequest, std::move(data));

        queueManager->PushToSendingData(std::move(sendMsg));
        Network::InitResponse initResponse;
        std::any data 
        auto sendMsg = std::make_unique<Network::Message>
                (Network::MessageType::InitRequest, std::move(data));

    } catch (const std::bad_weak_ptr &e) {
        //log
    }
}
void BeforeGameState::OnDestroy() {}

void BeforeGameState::Activate() {}
void BeforeGameState::Deactivate() {}

void BeforeGameState::Update(const sf::Time& l_time) {}
void BeforeGameState::Draw() {}
