#include <numeric>

#include "gameState.hpp"
#include "sharedContext.hpp"
#include "smartString.hpp"
#include "progressBar.hpp"
#include "textField.hpp"
#include "logger.hpp"
#include "exceptions.hpp"

GameState::GameState(std::weak_ptr<SharedContext> l_context)
    : BaseState(l_context) {}

void GameState::OnCreate() {
    try {
        auto context = GetSharedContext();
        auto eMgr = GetEventManager();

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

        auto progBar1 = context->sharedElements.find(ElementName::ProgressBar1);
        if (progBar1 != context->sharedElements.end()) {
            _elements.emplace(progBar1->first, progBar1->second);
        }
        auto progBar2 = context->sharedElements.find(ElementName::ProgressBar2);
        if (progBar2 != context->sharedElements.end()) {
            _elements.emplace(progBar2->first, progBar2->second);
        }
        auto progBar3 = context->sharedElements.find(ElementName::ProgressBar3);
        if (progBar3 != context->sharedElements.end()) {
            _elements.emplace(progBar3->first, progBar3->second);
        }
        auto progBar4 = context->sharedElements.find(ElementName::ProgressBar4);
        if (progBar4 != context->sharedElements.end()) {
            _elements.emplace(progBar4->first, progBar4->second);
        }
        auto progBar5 = context->sharedElements.find(ElementName::ProgressBar5);
        if (progBar5 != context->sharedElements.end()) {
            _elements.emplace(progBar5->first, progBar5->second);
        }

        auto lambdaQuit = [this]([[maybe_unused]] EventDetails& l_details) { this->Menu(); };
        eMgr->AddCallback(StateType::Game, "Key_Escape", lambdaQuit);

        auto lambdaTextEntered = [this]([[maybe_unused]] EventDetails& l_details) {
                this->TextEntered(l_details);
        };

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
        auto sendMsg = std::make_unique<Network::Message>(
                    Network::MessageType::ValidateWrittenTextRequest, textRequest);
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
        auto queueManager = GetQueueManager();

        Network::LeaveRoomRequest leaveRequest = {context->uuid};
        auto sendMsg = std::make_unique<Network::Message>(Network::MessageType::LeaveRoomRequest,
                leaveRequest);
        queueManager->PushToSendingData(std::move(sendMsg));

        std::unique_ptr<Network::Message> recvMsg = nullptr;
        while (!recvMsg) {
        recvMsg = queueManager->PopReceivedData();
        }

        if (recvMsg->GetMessageType() == Network::MessageType::LeaveRoomResponse) {
            context->sharedElements.erase(ElementName::SmartString);
            _elements.erase(ElementName::SmartString);
            stateMgr->SwitchTo(StateType::MainMenu);
            stateMgr->Remove(StateType::Game);
        }

    } catch (const std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[game - menu] " << e.what();
        return;
    } catch (const InvalidResponse& e) {
        BOOST_LOG_TRIVIAL(error) << "[game - menu] " << e.what();
    } catch (const std::bad_any_cast& e) {
        BOOST_LOG_TRIVIAL(error) << "[game - textentered] " << e.what();
    }
}

void GameState::UpdatePosition(const std::unordered_map<
        std::string, Network::PlayerInfo>& l_players) {

    try {
    auto context = GetSharedContext();
    auto itr = l_players.find(context->playerId);
    if (itr == l_players.end()) {
        //log
    }
    std::vector<std::pair<std::string, Network::PlayerInfo>> playersPositions;
    for (auto& [playerId, playerInfo]: l_players) {
        playersPositions.emplace_back(playerId, playerInfo);
    }
    std::sort(playersPositions.begin(), playersPositions.end(),[](const std::pair<std::string, Network::PlayerInfo>& lhs, const std::pair<std::string, Network::PlayerInfo>& rhs) {
        return lhs.second.position > rhs.second.position;
    });

    switch (playersPositions.size()) {
        case 1:
            refreshBar(playersPositions[0], ElementName::ProgressBar1);
            break;
        case 2:
            refreshBar(playersPositions[0], ElementName::ProgressBar1);
            refreshBar(playersPositions[1], ElementName::ProgressBar2);
            break;
        case 3:
            refreshBar(playersPositions[0], ElementName::ProgressBar1);
            refreshBar(playersPositions[1], ElementName::ProgressBar2);
            refreshBar(playersPositions[2], ElementName::ProgressBar3);
            break;
        case 4:
            refreshBar(playersPositions[0], ElementName::ProgressBar1);
            refreshBar(playersPositions[1], ElementName::ProgressBar2);
            refreshBar(playersPositions[2], ElementName::ProgressBar3);
            refreshBar(playersPositions[3], ElementName::ProgressBar4);
            break;
        case 5:
            refreshBar(playersPositions[0], ElementName::ProgressBar1);
            refreshBar(playersPositions[1], ElementName::ProgressBar2);
            refreshBar(playersPositions[2], ElementName::ProgressBar3);
            refreshBar(playersPositions[3], ElementName::ProgressBar4);
            refreshBar(playersPositions[4], ElementName::ProgressBar5);
            break;
        default:
            refreshBar(playersPositions[0], ElementName::ProgressBar1);
            refreshBar(playersPositions[1], ElementName::ProgressBar2);
            refreshBar(playersPositions[2], ElementName::ProgressBar3);
            refreshBar(playersPositions[3], ElementName::ProgressBar4);
            refreshBar(playersPositions[4], ElementName::ProgressBar5);
            break;
    }

    auto it = std::find_if(playersPositions.begin(), playersPositions.end(), [&](const auto& lhs) {
        if (context->playerId == lhs.first) {
        return true;
        }
        return false;
    });

    _playerPosition = std::distance(playersPositions.begin(), it) + 1;

    } catch (std::bad_weak_ptr& e) {
        //log
    }
}

void GameState::refreshBar(std::pair<std::string, Network::PlayerInfo>& l_player, ElementName l_elementName) {
    try {
        auto context = GetSharedContext();

        auto itrProgBar = _elements.find(l_elementName);
        if (itrProgBar == _elements.end()) {
            BOOST_LOG_TRIVIAL(error) << "[game - refreshBar] " << "bar not found";
        }

        auto itrStr = _elements.find(ElementName::SmartString);
        if (itrStr == _elements.end()) {
            BOOST_LOG_TRIVIAL(error) << "[game - refreshBar] " << "smartstring not found";
        }

        auto str = std::dynamic_pointer_cast<SmartString>(itrStr->second);
        auto progBar = std::dynamic_pointer_cast<ProgressBar>(itrProgBar->second);

        if (l_player.first == context->playerId) {
            progBar->SetPlayerStyle();
        } else {
            progBar->SetEnemyStyle();
        }
        progBar->Update(static_cast<float>(l_player.second.position) /
                static_cast<float>(str->GetStringSize()));

        if (l_elementName == ElementName::ProgressBar1) {
            progBar->SetText("#1 " + l_player.second.name);
        }
        if (l_elementName == ElementName::ProgressBar2) {
            progBar->SetText("#2 " + l_player.second.name);
        }
        if (l_elementName == ElementName::ProgressBar3) {
            progBar->SetText("#3 " + l_player.second.name);
        }
        if (l_elementName == ElementName::ProgressBar4) {
            progBar->SetText("#4 " + l_player.second.name);
        }
        if (l_elementName == ElementName::ProgressBar5) {
            progBar->SetText("#5 " + l_player.second.name);
        }

    } catch (std::bad_weak_ptr& e) {
        //log
    }
}

void GameState::GetPlayerPosition() {
    try {
        auto context = GetSharedContext();

        auto playerPos = std::make_shared<TextField>(ElementName::PlayerPosition,
                context, "textField.json", std::to_string(_playerPosition));
        context->sharedElements.emplace(ElementName::PlayerPosition, playerPos);

        auto posText = std::make_shared<TextField>(ElementName::PlayerPositionText,
                context, "textField.json", "Pos.");
        context->sharedElements.emplace(ElementName::PlayerPositionText, posText);

    } catch (std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[game - getplayerposition] " << e.what();
    }
}

void GameState::GetAverageSpeed() {
    try {
        auto context = GetSharedContext();

        auto averageSpeed = std::make_shared<TextField>(ElementName::AverageSpeed,
                context, "textField.json", std::to_string(static_cast<size_t>(_averageSpeed)));
        context->sharedElements.emplace(ElementName::AverageSpeed, averageSpeed);

        auto averageSpeedTxt = std::make_shared<TextField>(ElementName::AverageSpeedText,
                context, "textField.json", "Speed");
        context->sharedElements.emplace(ElementName::AverageSpeedText, averageSpeedTxt);

    } catch (std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[game - getaveragespeed] " << e.what();
    }
}

void GameState::CountAverageSpeed(const double l_speed) {
    _currentSpeed.push_back(l_speed);
    auto sum = std::accumulate(_currentSpeed.begin(), _currentSpeed.end(), 0.0);
    _averageSpeed = sum / _currentSpeed.size();
}

void GameState::OnDestroy() {
    try {
        _elements.erase(ElementName::SmartString);
        auto context = GetSharedContext();
        context->sharedElements.erase(ElementName::SmartString);

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

            UpdatePosition(roomStatusResponse.playersInfo);

            auto itr = roomStatusResponse.playersInfo.find(context->playerId);
            if (itr == roomStatusResponse.playersInfo.end()) {
                BOOST_LOG_TRIVIAL(error) << "[game - checkroomstatus] " << "player uuid not found";
                return;
            }
            CountAverageSpeed(itr->second.speed);
            _textPosition = itr->second.position;
            //UpdatePlayerPosition(roomStatusResponse.playersInfo);

            if (roomStatusResponse.roomStatus == Network::RoomStatus::End ||
                    itr->second.status == Network::PlayerInfo::Status::Finish) {
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

    auto progBar1 = _elements.find(ElementName::ProgressBar1);
    if (progBar1 != _elements.end()) {
        drawElement(ElementName::ProgressBar1);
    }
    auto progBar2 = _elements.find(ElementName::ProgressBar2);
    if (progBar2 != _elements.end()) {
        drawElement(ElementName::ProgressBar2);
    }
    auto progBar3 = _elements.find(ElementName::ProgressBar3);
    if (progBar3 != _elements.end()) {
        drawElement(ElementName::ProgressBar3);
    }
    auto progBar4 = _elements.find(ElementName::ProgressBar4);
    if (progBar4 != _elements.end()) {
        drawElement(ElementName::ProgressBar4);
    }
    auto progBar5 = _elements.find(ElementName::ProgressBar5);
    if (progBar5 != _elements.end()) {
        drawElement(ElementName::ProgressBar5);
    }
}

void GameState::Activate() {}
void GameState::Deactivate() {}
