#include <any>

#include "beforeGameState.hpp"
#include "exception.hpp"


BeforeGameState::BeforeGameState(std::weak_ptr<StateManager> l_stateManager)
    : BaseState(l_stateManager) {}

BeforeGameState::~BeforeGameState() {}

void BeforeGameState::OnCreate() {
    try {
        std::shared_ptr<StateManager> stateMgr(_stateMgr);
        std::shared_ptr<SharedContext> context(stateMgr->GetContext());

        Connect();
        Init();
        StartGameSession();

    } catch (const std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - oncreate] " << e.what();
    }
}

void BeforeGameState::StartGameSession() {
    try {
        std::shared_ptr<StateManager> stateMgr(_stateMgr);
        std::shared_ptr<SharedContext> context(stateMgr->GetContext());
        std::shared_ptr<Network::INetworkManager> networkManager(context->networkManager);
        std::shared_ptr<Network::Client::IQueueManager>queueManager(context->queueManager);

        Network::StartGameSessionRequest startGameRequest;
        std::any data = startGameRequest;
        auto sendMsg = std::make_unique<Network::Message>
                (Network::MessageType::ConnectRequest, std::move(data));
        queueManager->PushToSendingData(std::move(sendMsg));

        Network::StartGameSessionResponse connectResponse;
        std::unique_ptr<Network::Message> recvMsg = nullptr;
        while (!recvMsg) {
            recvMsg = queueManager->PopReceivedData()
        }

        if (recvMsg->GetMessageType() == Network::MessageType::ConnectResponse) {
            auto data = recvMsg->ExtractData();
            auto connectResponse = std::any_cast<Network::ConnectResponse>(data);
            CheckNetStatus(connectResponse.status, connectResponse.error);
        }


    } catch (const std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - connect] " << e.what();
    } catch (const std::bad_any_cast& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - connect] " << e.what();
    } catch (const InvalidResponse& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - connect] " << e.what();
    }
}

void BeforeGameState::Connect() {
    try {
        std::shared_ptr<StateManager> stateMgr(_stateMgr);
        std::shared_ptr<SharedContext> context(stateMgr->GetContext());
        std::shared_ptr<Network::INetworkManager> networkManager(context->networkManager);
        std::shared_ptr<Network::Client::IQueueManager>queueManager(context->queueManager);

        Network::ConnectRequest connectRequest;
        std::any data = connectRequest;
        auto sendMsg = std::make_unique<Network::Message>
                (Network::MessageType::ConnectRequest, std::move(data));
        queueManager->PushToSendingData(std::move(sendMsg));

        Network::ConnectResponse connectResponse;
        std::unique_ptr<Network::Message> recvMsg = nullptr;
        while (!recvMsg) {
            recvMsg = queueManager->PopReceivedData()
        }

        if (recvMsg->GetMessageType() == Network::MessageType::ConnectResponse) {
            auto data = recvMsg->ExtractData();
            auto connectResponse = std::any_cast<Network::ConnectResponse>(data);
            CheckNetStatus(connectResponse.status, connectResponse.error);
        }

    } catch (const std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - connect] " << e.what();
    } catch (const std::bad_any_cast& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - connect] " << e.what();
    } catch (const InvalidResponse& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - connect] " << e.what();
    }
}

void BeforeGameState::Init() {
    try {
        std::shared_ptr<StateManager> stateMgr(_stateMgr);
        std::shared_ptr<SharedContext> context(stateMgr->GetContext());
        std::shared_ptr<Network::INetworkManager> networkManager(context->networkManager);
        std::shared_ptr<Network::Client::IQueueManager>queueManager(context->queueManager);

        networkManager->Connect();
        Network::InitRequest initRequest;
        std::any data = initRequest;
        auto sendMsg = std::make_unique<Network::Message>
                (Network::MessageType::InitRequest, std::move(data));
        queueManager->PushToSendingData(std::move(sendMsg));

        Network::InitResponse initResponse;
        std::unique_ptr<Network::Message> recvMsg = nullptr;
        while (!recvMsg) {
            recvMsg = queueManager->PopReceivedData()
        }

        if (recvMsg->GetMessageType() == Network::MessageType::InitResponse) {
            auto data = recvMsg->ExtractData();
            auto initResponse = std::any_cast<Network::InitResponse>(data);
            CheckNetStatus(initResponse.status, initResponse.error);
            context->uuid = initResponse.id;
        }

    } catch (const std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - init] " << e.what();
    } catch (const std::bad_any_cast& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - init] " << e.what();
    } catch (const InvalidResponse& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - init] " << e.what();
    }
}

void BeforeGameState::OnDestroy() {}

void BeforeGameState::Activate() {}
void BeforeGameState::Deactivate() {}

void BeforeGameState::Update(const sf::Time& l_time) {}
void BeforeGameState::Draw() {}
