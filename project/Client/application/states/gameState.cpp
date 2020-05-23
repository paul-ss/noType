#include "numeric"

#include "gameState.hpp"
#include "sharedContext.hpp"
#include "smartString.hpp"
#include "logger.hpp"
#include "exceptions.hpp"

GameState::GameState(std::weak_ptr<SharedContext> l_context)
    : BaseState(l_context) {}

void GameState::OnCreate() {
    try {
        auto context = GetSharedContext();
        auto eMgr = GetEventManager();

        auto smartString = context->sharedElements.find(ElementName::SmartString);
        if (smartString == context->sharedElements.end()) {
            BOOST_LOG_TRIVIAL(error) << "[game - textentered] " << "smartstring not found";
        }
        _elements.emplace(smartString->first, smartString->second);

        // add mute, back to menu

        auto lambdaQuit = [this]([[maybe_unused]] EventDetails& l_details) { this->GoToMenu(); };
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
    auto input = l_details.keyCode;
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
            _position += textResponse.rightCount;
        }
    } catch (const std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[game - textentered] " << e.what();
    } catch (const InvalidResponse& e) {
        BOOST_LOG_TRIVIAL(error) << "[game - textentered] " << e.what();
    } catch (const std::bad_any_cast& e) {
        BOOST_LOG_TRIVIAL(error) << "[game - textentered] " << e.what();
    }
}

void GameState::GoToMenu() {
    try {
        auto context = GetSharedContext();
        auto stateMgr = GetStateManager();
        stateMgr->SwitchTo(StateType::MainMenu);
        stateMgr->Remove(StateType::Game);

    } catch (const std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[game - gotomenu] " << e.what();
        return;
    }
}

// size_t GameState::GetLeaderPosition() {}

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
        std::any data = roomRequest;
        auto sendMsg = std::make_unique<Network::Message>
                (Network::MessageType::RoomStatusRequest, std::move(data));
        queueManager->PushToSendingData(std::move(sendMsg));

        Network::RoomStatusResponse roomResponse;
        std::unique_ptr<Network::Message> recvMsg = nullptr;
        while (!recvMsg) {
            recvMsg = queueManager->PopReceivedData();
        }
        if (recvMsg->GetMessageType() == Network::MessageType::RoomStatusResponse) {
            auto data = recvMsg->ExtractData();
            auto roomStatusResponse = std::any_cast<Network::RoomStatusResponse>(data);
            auto itr = roomStatusResponse.playersInfo.find(context->uuid);
            if (itr != roomStatusResponse.playersInfo.end()) {
                _averageSpeed = CountAverageSpeed(itr->second.speed);
                _position = itr->second.position;
            } else {
                BOOST_LOG_TRIVIAL(error) << "[game - checkroomstatus] " << "player uuid not found";
            }
        }

    } catch (const std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[game - mainmenu] " << e.what();
        return;
    } catch (const InvalidResponse& e) {
        BOOST_LOG_TRIVIAL(error) << "[game - checkroomstatus] " << "Invalid respone"
                << e.what();
    }
}

void GameState::Draw() {
    drawElement(ElementName::Filler);
    drawElement(ElementName::SmartString);
}

void GameState::Activate() {}
void GameState::Deactivate() {}
