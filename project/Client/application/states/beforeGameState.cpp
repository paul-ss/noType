#include "beforeGameState.hpp"

#include <any>

BeforeGameState::BeforeGameState(std::weak_ptr<StateManager> l_stateManager)
    : BaseState(l_stateManager) {}

BeforeGameState::~BeforeGameState() {}

void BeforeGameState::OnCreate() {
    try {
        std::shared_ptr<StateManager> stateMgr(_stateMgr);
        std::shared_ptr<SharedContext> context(stateMgr->GetContext());
        std::shared_ptr<Network::INetworkManager> networkManager(context->networkManager);
        std::shared_ptr<Network::Client::IQueueManager>queueManager(context->queueManager);

        networkManager->Connect();
        Network::InitRequest initRequest;
        std::any sendData = initRequest;
        auto sendMsg = std::make_unique<Network::Message>
                (Network::MessageType::InitRequest, std::move(data));

        queueManager->PushToSendingData(std::move(sendMsg));

        std::unique_ptr<Network::Message> recvMsg = nullptr;
        while (!recvMsg) {
          recvMsg = queueManager->PopReceivedData()
        }

        if (recvMsg->GetMessageType() == Network::MessageType::InitResponse) {
          auto data = recvMsg->ExtractData();
          auto initResponse = std::any_cast<Network::InitResponse>(data);
        }

    } catch (const std::bad_weak_ptr &e) {
        //log
    } catch (const std::bad_any_cast &e) {
        //log
    }
}
void BeforeGameState::OnDestroy() {}

void BeforeGameState::Activate() {}
void BeforeGameState::Deactivate() {}

void BeforeGameState::Update(const sf::Time& l_time) {}
void BeforeGameState::Draw() {}
