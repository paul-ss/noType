#include <any>

#include "beforeGameState.hpp"
#include "sharedContext.hpp"
#include "smartString.hpp"
#include "label.hpp"
#include "textField.hpp"
#include "logger.hpp"
#include "exceptions.hpp"

#include <thread>

BeforeGameState::BeforeGameState(std::weak_ptr<SharedContext> l_context)
    : BaseState(l_context), _waitTime(0) {}

void BeforeGameState::OnCreate() {
    try {
        Init();
        Connect();
        StartGameSession();
        GetText();

        // TODO add binding mute, exit
        auto context = GetSharedContext();
        auto fillerItr = context->sharedElements.find(ElementName::Filler);
        if (fillerItr != context->sharedElements.end()) {
            _elements.emplace(fillerItr->first, fillerItr->second);
        } else {
            BOOST_LOG_TRIVIAL(error) << "[beforeGameState - oncreate] " << "filler not found";
        }


    } catch (const std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - oncreate] " << e.what();
    }
}

void BeforeGameState::Init() {
    try {
        auto context = GetSharedContext();
        auto queueManager = GetQueueManager();

        Network::InitRequest initRequest;
        auto sendMsg = std::make_unique<Network::Message>
                (Network::MessageType::InitRequest, initRequest);
        queueManager->PushToSendingData(std::move(sendMsg));

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

void BeforeGameState::Connect() {
    try {
        auto context = GetSharedContext();
        auto queueManager = GetQueueManager();

        Network::ConnectRequest connectRequest = {context->uuid};
        auto sendMsg = std::make_unique<Network::Message>
                (Network::MessageType::ConnectRequest, connectRequest);
        queueManager->PushToSendingData(std::move(sendMsg));

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

void BeforeGameState::StartGameSession() {
    try {
        auto context = GetSharedContext();
        auto renderWindow = GetRenderWindow();
        auto queueManager = GetQueueManager();

        Network::StartGameSessionRequest startGameRequest = {context->uuid};
        auto sendMsg = std::make_unique<Network::Message>
                (Network::MessageType::StartGameSessionRequest, startGameRequest);
        queueManager->PushToSendingData(std::move(sendMsg));

        std::unique_ptr<Network::Message> recvMsg = nullptr;
        while (!recvMsg) {
            recvMsg = queueManager->PopReceivedData();
        }

        if (recvMsg->GetMessageType() == Network::MessageType::StartGameSessionResponse) {
            auto data = recvMsg->ExtractData();
            auto startGameSessionResponse = std::any_cast<Network::StartGameSessionResponse>(data);
            checkNetStatus(startGameSessionResponse.status, startGameSessionResponse.error);
            _waitTime = startGameSessionResponse.waitTime + 500;
            auto windowSize = renderWindow->getSize();
            sf::Vector2f windowCenter(windowSize.x * 0.5, windowSize.y * 0.5);
            auto timeToStart = std::make_shared<TextField>(ElementName::TimeToStart,
                    context, windowCenter, "textField.json", std::to_string(_waitTime));

            timeToStart->SetText(std::to_string(_waitTime));
            context->playerId = startGameSessionResponse.playerId;
            auto timeSize = timeToStart->GetSize();
            sf::Vector2f timePosition((windowSize.x * 0.5) - timeSize.x * 0.5, (windowSize.y * 0.5f));
            timeToStart->SetPosition(timePosition);
            _elements.emplace(ElementName::TimeToStart, timeToStart);
        }

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

void BeforeGameState::GetText() {
    try {
        auto context = GetSharedContext();
        auto renderWindow = GetRenderWindow();
        auto queueManager = GetQueueManager();

        Network::GetTextRequest getTextRequest = {context->uuid};
        auto sendMsg = std::make_unique<Network::Message>
          (Network::MessageType::GetTextRequest, getTextRequest);
        queueManager->PushToSendingData(std::move(sendMsg));

        std::unique_ptr<Network::Message> recvMsg = nullptr;
        while (!recvMsg) {
          recvMsg = queueManager->PopReceivedData();
        }

        if (recvMsg->GetMessageType() == Network::MessageType::GetTextResponse) {
          auto data = recvMsg->ExtractData();
          auto getTextResponse = std::any_cast<Network::GetTextResponse>(data);
          checkNetStatus(getTextResponse.status, getTextResponse.error);

          // Create text string and share it with game state
          auto windowSize = renderWindow->getSize();
          auto smartString = std::make_shared<SmartString>(ElementName::SmartString,
                                                           context,
                                                           sf::Vector2f((windowSize.x * 0.5), (windowSize.y * 0.5f)),
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

void BeforeGameState::Game() {
    try {
        auto stateMgr = GetStateManager();
        stateMgr->SwitchTo(StateType::Game);
        stateMgr->Remove(StateType::BeforeGame);

    } catch (const std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - game] " << e.what();
    }
}

void BeforeGameState::BackToMenu() {
    try {
        auto stateMgr = GetStateManager();
        stateMgr->SwitchTo(StateType::MainMenu);
        stateMgr->Remove(StateType::BeforeGame);

    } catch (const std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - backtomenu] " << e.what();
    }
}

void BeforeGameState::Update(const sf::Time& l_time) {
    try {
        auto stateMgr = GetStateManager();
        _waitTime -= l_time.asMilliseconds();
        if (_waitTime >= 0) {
            auto itr = _elements.find(ElementName::TimeToStart);
            if (itr == _elements.end()) {
                return;
            }
            auto waitTimeInSeconds = std::max<int>(_waitTime / 1000, 0);
            itr->second->SetText(std::to_string(waitTimeInSeconds));
        } else {
            if (isGame()) {
                Game();
            } else {
                BOOST_LOG_TRIVIAL(error) << "[beforeGameState - update] " << "Internal server error. Back to menu.";
                BackToMenu();
            }
        }

    } catch (std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - update] " << e.what();
    }
}

void BeforeGameState::Draw() {
    drawElement(ElementName::Filler);
    drawElement(ElementName::TimeToStart);
}

bool BeforeGameState::isGame() {
    try {
        auto queueManager = GetQueueManager();
        auto context = GetSharedContext();

        while (true) {
          Network::RoomStatusRequest roomStatusRequest = {context->uuid};
          auto sendMsg = std::make_unique<Network::Message>
            (Network::MessageType::RoomStatusRequest, roomStatusRequest);
          queueManager->PushToSendingData(std::move(sendMsg));

          std::unique_ptr<Network::Message> recvMsg = nullptr;
          while (!recvMsg) {
            recvMsg = queueManager->PopReceivedData();
          }

          if (recvMsg->GetMessageType() == Network::MessageType::RoomStatusResponse) {
            auto roomStatusResponse = std::any_cast<Network::RoomStatusResponse>(recvMsg->ExtractData());
            checkNetStatus(roomStatusResponse.status, roomStatusResponse.error);
            if (roomStatusResponse.roomStatus == Network::RoomStatus::Play) {
              return true;
            } else {
              return false;
            }
          } else {
            return false;
          }
          std::this_thread::sleep_for(std::chrono::microseconds(300));
          std::cout << "continue waiting" << std::endl;
        }
    } catch (const std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - isGame] " << e.what();
    } catch (const std::bad_any_cast& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - isGame] " << e.what();
    } catch (const InvalidResponse& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - isGame] " << e.what();
    } catch (const ConnectionFailure& e) {
        BOOST_LOG_TRIVIAL(error) << "[beforeGameState - isGame] " << e.what();
    }
    return false;
}

void BeforeGameState::OnDestroy() {}
void BeforeGameState::Activate() {}
void BeforeGameState::Deactivate() {}
