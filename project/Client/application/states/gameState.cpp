#include <numeric>

#include "gameState.hpp"
#include "sharedContext.hpp"
#include "smartString.hpp"
#include "progressBar.hpp"
#include "logger.hpp"
#include "exceptions.hpp"

GameState::GameState(std::weak_ptr<SharedContext> l_context)
    : BaseState(l_context) {}

void GameState::OnCreate() {
    try {
        auto context = GetSharedContext();
        auto eMgr = GetEventManager();
        auto renderWindow = GetRenderWindow();

        auto smartStringItr = context->sharedElements.find(ElementName::SmartString);
        if (smartStringItr != context->sharedElements.end()) {
            _elements.emplace(smartStringItr->first, smartStringItr->second);
        } else {
            BOOST_LOG_TRIVIAL(error) << "[game - textentered] " << "smartstring not found";
        }

        auto fillerItr = context->sharedElements.find(ElementName::Filler);
        if (fillerItr != context->sharedElements.end()) {
            _elements.emplace(fillerItr->first, fillerItr->second);
        } else {
            BOOST_LOG_TRIVIAL(error) << "[beforeGameState - oncreate] " << "filler not found";
        }

        //auto windowSize = renderWindow->getSize();
        //auto pb = std::make_shared<ProgressBar>(ElementName::LeaderPosition, context, sf::Vector2f(0, 0), "progressBar.json");
        //auto pbSize = pb->GetSize();
        //sf::Vector2f pbPosition(0,0);
        //pb->SetPosition(pbPosition);
        //_elements.emplace(ElementName::LeaderPosition, pb);

        // add mute, back to menu button
        auto lambdaQuit = [this]([[maybe_unused]] EventDetails& l_details) { this->Menu(); };
        eMgr->AddCallback(StateType::Game, "Key_Escape", lambdaQuit);

        auto lambdaTextEntered = [this]([[maybe_unused]] EventDetails& l_details) { this->TextEntered(l_details); };
        eMgr->AddCallback(StateType::Game, "Text_Entered", lambdaTextEntered);
        eMgr->AddCallback(StateType::Game, "Text_Deleted", lambdaTextEntered);

    } catch (const std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[game - oncreate] " << e.what();
    }
}

void GameState::TextEntered(EventDetails& l_details) {
    auto itr = _elements.find(ElementName::SmartString);
    if (itr == _elements.end()) {
        BOOST_LOG_TRIVIAL(error) << "[game - textentered] " << "smartstring not found";
    }

    auto smartString = std::dynamic_pointer_cast<SmartString>(itr->second);
    char input = l_details.textEntered;
    auto validatedBlock = smartString->Validate(input);
    if (!validatedBlock.empty()) {
        UpdatePosition(validatedBlock);
    }
}

void GameState::UpdatePosition(const std::string& l_validatedBlock) {
    try {
        auto context = GetSharedContext();
        auto queueManager = GetQueueManager();

        Network::ValidateWrittenTextRequest textRequest = {context->uuid, l_validatedBlock};
        auto sendMsg = std::make_unique<Network::Message>(Network::MessageType::ValidateWrittenTextRequest, textRequest);
        queueManager->PushToSendingData(std::move(sendMsg));

        std::unique_ptr<Network::Message> recvMsg = nullptr;
        while (!recvMsg) {
        recvMsg = queueManager->PopReceivedData();
        }

        if (recvMsg->GetMessageType() == Network::MessageType::ValidateWrittenTextResponse) {
            auto data = recvMsg->ExtractData();
            auto textResponse = std::any_cast<Network::ValidateWrittenTextResponse>(data);
            checkNetStatus(textResponse.status, textResponse.error);
            _textPosition += textResponse.rightCount;
        }
    } catch (const std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[game - textentered] " << e.what();
    } catch (const InvalidResponse& e) {
        BOOST_LOG_TRIVIAL(error) << "[game - textentered] " << e.what();
    } catch (const std::bad_any_cast& e) {
        BOOST_LOG_TRIVIAL(error) << "[game - textentered] " << e.what();
    }
}

void GameState::Menu() {
    try {
        auto context = GetSharedContext();
        auto stateMgr = GetStateManager();

        context->sharedElements.erase(ElementName::SmartString);
        stateMgr->SwitchTo(StateType::MainMenu);
        stateMgr->Remove(StateType::Game);

    } catch (const std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[game - gotomenu] " << e.what();
        return;
    }
}

void GameState::UpdateLeaderPosition(const std::unordered_map<std::string, PlayerInfo>& l_players) {
    size_t currPos = 0;
    for (auto& itr : l_players) {
        if (currPos < itr.second.position) {
            currPos = itr.second.position;
        }
    }
    auto itrPb = _elements.find(ElementName::LeaderPosition);
    if (itrPb == _elements.end()) {
        BOOST_LOG_TRIVIAL(error) << "[game - updateleaderposition] " << "leaderposition not found";
    }
    auto itrStr = _elements.find(ElementName::SmartString);
    if (itrStr == _elements.end()) {
        BOOST_LOG_TRIVIAL(error) << "[game - updateleaderposition] " << "leaderposition not found";
    }
    auto str = std::dynamic_pointer_cast<SmartString>(itrStr->second);
    //itrPb->second->Update(currPos / str->GetStringSize());
    itrPb->second->Update(50.0f);
}

size_t GameState::UpdatePlayerPosition(const std::unordered_map<std::string, PlayerInfo>& l_players) {
    try {
    auto context = GetSharedContext();
    auto itr = l_players.find(context->uuid);
    if (itr == l_players.end()) {
        //log
        return 0;
    }
    size_t
    } catch (std::bad_weak_ptr& e) {
        //log
    }
}

size_t GameState::GetPlayerPosition() {
    try {

    } catch (const std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[game - getplayerposition] " << e.what();
    }
}

void GameState::GetAverageSpeed() {
    auto windowSize = renderWindow->getSize();
    sf::Vector2f windowCenter(windowSize.x * 0.5, windowSize.y * 0.5);
    auto timeToStart = std::make_shared<TextField>(ElementName::TimeToStart,
            context, windowCenter, "textField.json", std::to_string(_waitTime));
}

double GameState::CountAverageSpeed(const double l_speed) {
    _currentSpeed.push_back(l_speed);
    auto sum = std::accumulate(_currentSpeed.begin(), _currentSpeed.end(), 0.0);
    return sum / _currentSpeed.size();
}

void GameState::OnDestroy() {
    try {
        auto eMgr = GetEventManager();
        eMgr->RemoveCallback(StateType::Game, "Key_Escape");
        eMgr->RemoveCallback(StateType::Game, "Text_Entered");
        eMgr->RemoveCallback(StateType::Game, "Text_Deleted");

    } catch (const std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[game - ondestroy] " << e.what();
    }
}

void GameState::Update(const sf::Time& time) {
    _timePass += time.asSeconds();
    if(_timePass >= 1.0f) {
        _timePass = 0;
        CheckRoomStatus();
    }
}

void GameState::CheckRoomStatus() {
    try {
        auto context = GetSharedContext();
        auto queueManager = GetQueueManager();

        Network::RoomStatusRequest roomRequest = {context->uuid};
        auto sendMsg = std::make_unique<Network::Message>
                (Network::MessageType::RoomStatusRequest, roomRequest);
        queueManager->PushToSendingData(std::move(sendMsg));

        Network::RoomStatusResponse roomResponse;
        std::unique_ptr<Network::Message> recvMsg = nullptr;
        while (!recvMsg) {
            recvMsg = queueManager->PopReceivedData();
        }
        if (recvMsg->GetMessageType() == Network::MessageType::RoomStatusResponse) {
            auto data = recvMsg->ExtractData();
            auto roomStatusResponse = std::any_cast<Network::RoomStatusResponse>(data);

            UpdateLeaderPosition(roomStatusResponse.playersInfo);

            auto itr = roomStatusResponse.playersInfo.find(context->playerId);
            if (itr == roomStatusResponse.playersInfo.end()) {
                BOOST_LOG_TRIVIAL(error) << "[game - checkroomstatus] " << "player uuid not found";
                return;
            }
            _averageSpeed = CountAverageSpeed(itr->second.speed);
            _textPosition = itr->second.position;
            _playerPosition = UpdatePlayerPosition(roomStatusResponse.playersInfo);

            if (roomStatusResponse.roomStatus == Network::RoomStatus::End ||
                    itr->second.status == PlayerInfo::Status::Finish) {
                AfterGame();
            }
        }

    } catch (const std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[game - mainmenu] " << e.what();
        return;
    } catch (const InvalidResponse& e) {
        BOOST_LOG_TRIVIAL(error) << "[game - checkroomstatus] " << "Invalid respone" << e.what();
    }
}

void GameState::AfterGame() {
    try {
        GetAverageSpeed();
        GetPlayerPosition();
        auto stateMgr = GetStateManager();
        stateMgr->SwitchTo(StateType::AfterGame);
        stateMgr->Remove(StateType::Game);

    } catch (const std::bad_weak_ptr &e) {
        BOOST_LOG_TRIVIAL(error) << "[intro - continue] " << e.what();
    }
}

void GameState::Draw() {
    drawElement(ElementName::Filler);
    drawElement(ElementName::SmartString);
    //drawElement(ElementName::LeaderPosition);
}

void GameState::Activate() {}
void GameState::Deactivate() {}
