#include "numeric"

#include "gameState.hpp"
#include "sharedContext.hpp"
#include "logger.hpp"
#include "exceptions.hpp"

GameState::GameState(std::weak_ptr<SharedContext> l_context)
    : BaseState(l_context) {}

void GameState::OnCreate() {
    try {
        auto context = GetSharedContext();
        auto eMgr = GetEventManager();

        auto lambdaQuit = [this](EventDetails& l_details) { this->GoToMenu(); };
        eMgr->AddCallback(StateType::Game, "Key_Escape", lambdaQuit);

        auto lambdaTextEntered = [this](EventDetails& l_details) { this->TextEntered(l_details); };
        eMgr->AddCallback(StateType::Game, "Text_Entered", lambdaTextEntered);
        eMgr->AddCallback(StateType::Game, "Text_Deleted", lambdaTextEntered);

    } catch (const std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[game - oncreate] " << e.what();
    }
}

void GameState::TextEntered(EventDetails& l_details) {
    try {

    } catch (const std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[game - textentered] " << e.what();
    }
}

void GameState::GoToMenu() {
    try {
        auto stateMgr = GetStateManager();
        stateMgr->SwitchTo(StateType::MainMenu);

    } catch (const std::bad_weak_ptr& e) {
        BOOST_LOG_TRIVIAL(error) << "[game - mainmenu] " << e.what();
        return;
    }
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
    if(_timePass < 5.0f) {
        CheckRoomStatus();
    }
}

void GameState::CheckRoomStatus() {
    try {
        auto context = GetSharedContext();
        auto queueManager = GetQueueManager();

        Network::RoomStatusRequest roomRequest;
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
    for (auto& element : _elements) {
        element.second->Draw();
    }
}

void GameState::Activate() {}
void GameState::Deactivate() {}
