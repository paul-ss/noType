#include <any>

#include "beforeGameState.hpp"
#include "sharedContext.hpp"
#include "smartString.hpp"
#include "label.hpp"
#include "textField.hpp"
#include "logger.hpp"
#include "exceptions.hpp"

BeforeGameState::BeforeGameState(std::weak_ptr<SharedContext> l_context)
    : BaseState(l_context), _waitTime(0) {}

void BeforeGameState::OnCreate() {
    try {
        Init();
        Connect();
        StartGameSession();
        GetText();

        // TODO add binding mute, exit

    } catch (const std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - oncreate] " << e.what();
    }
}

void BeforeGameState::GetText() {
    try {
        auto context = GetSharedContext();
        auto renderWindow = GetRenderWindow();
        auto queueManager = GetQueueManager();

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
            checkNetStatus(getTextResponse.status, getTextResponse.error);

            // Create text string and share it with game state
            auto windowSize = renderWindow->getSize();
            auto smartString = std::make_shared<SmartString>(ElementName::SmartString,
                    context, sf::Vector2f((windowSize.x * 0.5), (windowSize.y * 0.5f)),
                    "smartString.json", getTextResponse.text);

            sf::Vector2f smartStrPosition((windowSize.x * 0.5), (windowSize.y * 0.5f));
            smartString->SetPosition(smartStrPosition);
            context->sharedElements.emplace(ElementName::SmartString, smartString);
        }

    } catch (const std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - getText] " << e.what();
    } catch (const std::bad_any_cast& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - getText] " << e.what();
    } catch (const InvalidResponse& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - getText] " << e.what();
    } catch (const ConnectionFailure& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - init] " << e.what();
    }
}

void BeforeGameState::StartGameSession() {
    try {
        auto context = GetSharedContext();
        auto renderWindow = GetRenderWindow();
        auto queueManager = GetQueueManager();

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
            checkNetStatus(startGameSessionResponse.status, startGameSessionResponse.error);
            _waitTime = startGameSessionResponse.waitTime;
        }

        auto windowSize = renderWindow->getSize();
        auto timeToStart = std::make_shared<TextField>(ElementName::TimeToStart,
                context, sf::Vector2f(0, 0), "textField.json", std::to_string(_waitTime));

        timeToStart->SetText(std::to_string(_waitTime));
        auto timeSize = timeToStart->GetSize();
        sf::Vector2f timePosition((windowSize.x * 0.5) - timeSize.x * 0.5, (windowSize.y * 0.5f));
        timeToStart->SetPosition(timePosition);
        _elements.emplace(ElementName::TimeToStart, timeToStart);

    } catch (const std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - startgamesession] " << e.what();
    } catch (const std::bad_any_cast& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - startgamesession] " << e.what();
    } catch (const InvalidResponse& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - startgamesession] " << e.what();
    } catch (const ConnectionFailure& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - init] " << e.what();
    }
}

void BeforeGameState::Connect() {
    try {
        auto context = GetSharedContext();
        auto queueManager = GetQueueManager();

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
            checkNetStatus(connectResponse.status, connectResponse.error);
        }

    } catch (const std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - connect] " << e.what();
    } catch (const std::bad_any_cast& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - connect] " << e.what();
    } catch (const InvalidResponse& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - connect] " << e.what();
    } catch (const ConnectionFailure& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - init] " << e.what();
    }
}

void BeforeGameState::Init() {
    try {
        auto context = GetSharedContext();
        auto queueManager = GetQueueManager();

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
            checkNetStatus(initResponse.status, initResponse.error);
            context->uuid = initResponse.id;
        }

    } catch (const std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - init] " << e.what();
    } catch (const std::bad_any_cast& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - init] " << e.what();
    } catch (const InvalidResponse& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - init] " << e.what();
    } catch (const ConnectionFailure& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - init] " << e.what();
    }
}

void BeforeGameState::Game() {
    try {
        auto stateMgr = GetStateManager();
        stateMgr->SwitchTo(StateType::Game);

    } catch (const std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - init] " << e.what();
    }
}

void BeforeGameState::Update(const sf::Time& l_time) {
    _waitTime -= l_time.asSeconds();
    if (_waitTime <= 0) {
        Game();
    } else {
        auto itr = _elements.find(ElementName::TimeToStart);
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
