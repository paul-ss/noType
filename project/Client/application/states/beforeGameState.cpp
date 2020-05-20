#include <any>

#include "beforeGameState.hpp"
#include "exceptions.hpp"


BeforeGameState::BeforeGameState(std::weak_ptr<StateManager> l_stateManager)
    : BaseState(l_stateManager), _waitTime(0) {}

BeforeGameState::~BeforeGameState() {}

void BeforeGameState::OnCreate() {
    try {
        std::shared_ptr<StateManager> stateMgr(_stateMgr);
        std::shared_ptr<SharedContext> context(stateMgr->GetContext());

        Init();
        Connect();
        StartGameSession();
        GetText();

    } catch (const std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - oncreate] " << e.what();
    }
}

void BeforeGameState::GetText() {
    try {
        std::shared_ptr<StateManager> stateMgr(_stateMgr);
        std::shared_ptr<SharedContext> context(stateMgr->GetContext());
        std::shared_ptr<Network::INetworkManager> networkManager(context->networkManager);
        std::shared_ptr<Network::Client::IQueueManager>queueManager(context->queueManager);
        std::shared_ptr<Window>window(context->window);
        std::shared_ptr<sf::RenderWindow>renderWindow(window->GetRenderWindow());

        Network::GetTextRequest getTextRequest;
        std::any data = getTextRequest;
        auto sendMsg = std::make_unique<Network::Message>
                (Network::MessageType::GetTextRequest, std::move(data));
        queueManager->PushToSendingData(std::move(sendMsg));

        Network::GetTextResponse getTextResponse;
        std::unique_ptr<Network::Message> recvMsg = nullptr;
        while (!recvMsg) {
            recvMsg = queueManager->PopReceivedData();
        }

        if (recvMsg->GetMessageType() == Network::MessageType::GetTextResponse) {
            auto data = recvMsg->ExtractData();
            auto startGameSessionResponse = std::any_cast<Network::ConnectResponse>(data);
            CheckNetStatus(getTextResponse.status, getTextResponse.error);

            // create text string and share it with game state
            auto windowSize = renderWindow->getSize();
            auto smartString = std::make_shared<SmartString>(context,
                    sf::Vector2f((windowSize.x * 0.5), (windowSize.y * 0.5f)),
                    "smartString.json", getTextResponse.text);

            sf::Vector2f smartStrPosition((windowSize.x * 0.5), (windowSize.y * 0.5f));
            smartString->SetPosition(smartStrPosition);
            context->sharedElements.emplace("SmartString", smartString);
        }

    } catch (const std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - getText] " << e.what();
    } catch (const std::bad_any_cast& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - getText] " << e.what();
    } catch (const InvalidResponse& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - getText] " << e.what();
    }
}

void BeforeGameState::StartGameSession() {
    try {
        std::shared_ptr<StateManager> stateMgr(_stateMgr);
        std::shared_ptr<SharedContext> context(stateMgr->GetContext());
        std::shared_ptr<Network::INetworkManager> networkManager(context->networkManager);
        std::shared_ptr<Network::Client::IQueueManager>queueManager(context->queueManager);
        std::shared_ptr<Window>window(context->window);
        std::shared_ptr<sf::RenderWindow>renderWindow(window->GetRenderWindow());

        Network::StartGameSessionRequest startGameRequest;
        std::any data = startGameRequest;
        auto sendMsg = std::make_unique<Network::Message>
                (Network::MessageType::StartGameSessionRequest, std::move(data));
        queueManager->PushToSendingData(std::move(sendMsg));

        Network::StartGameSessionResponse startGameSessionResponse;
        std::unique_ptr<Network::Message> recvMsg = nullptr;
        while (!recvMsg) {
            recvMsg = queueManager->PopReceivedData();
        }

        if (recvMsg->GetMessageType() == Network::MessageType::StartGameSessionResponse) {
            auto data = recvMsg->ExtractData();
            auto startGameSessionResponse = std::any_cast<Network::StartGameSessionResponse>(data);
            CheckNetStatus(startGameSessionResponse.status, startGameSessionResponse.error);
            _waitTime = startGameSessionResponse.waitTime;
        }

        auto windowSize = renderWindow->getSize();
        auto timeToStart = std::make_shared<TextField>(context, sf::Vector2f(0, 0),
                "textField.json", std::to_string(_waitTime));

        timeToStart->SetText(std::to_string(_waitTime));
        auto timeSize = timeToStart->GetSize();
        sf::Vector2f timePosition((windowSize.x * 0.5) - timeSize.x * 0.5, (windowSize.y * 0.5f));
        timeToStart->SetPosition(timePosition);
        _elements.emplace("TimeToStart", timeToStart);

    } catch (const std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - startgamesession] " << e.what();
    } catch (const std::bad_any_cast& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - startgamesession] " << e.what();
    } catch (const InvalidResponse& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - startgamesession] " << e.what();
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
            recvMsg = queueManager->PopReceivedData();
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

        Network::InitRequest initRequest;
        std::any data = initRequest;
        auto sendMsg = std::make_unique<Network::Message>
                (Network::MessageType::InitRequest, std::move(data));
        queueManager->PushToSendingData(std::move(sendMsg));

        Network::InitResponse initResponse;
        std::unique_ptr<Network::Message> recvMsg = nullptr;
        while (!recvMsg) {
            recvMsg = queueManager->PopReceivedData();
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

void BeforeGameState::Game() {
    auto stateMgr = _stateMgr.lock();
    if (!stateMgr) {
        BOOST_LOG_TRIVIAL(error) << "Not valid state manager [menu - play]";
        return;
    }
    stateMgr->SwitchTo(StateType::Game);
}

void BeforeGameState::Update(const sf::Time& l_time) {
    _waitTime -= l_time.asSeconds();
    if (_waitTime <= 0) {
        Game();
    } else {
        auto itr = _elements.find("TimeToStart");
        if (itr == _elements.end()) {
            return;
        }
        itr->second->SetText(std::to_string(_waitTime));
    }
}

void BeforeGameState::Draw() {
    for (auto& element : _elements) {
        element.second->Draw();
    }
}

void BeforeGameState::OnDestroy() {}

void BeforeGameState::Activate() {}
void BeforeGameState::Deactivate() {}



