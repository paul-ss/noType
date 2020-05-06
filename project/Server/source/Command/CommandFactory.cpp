//
// Created by paul_s on 17.04.2020.
// Modified by foxyuiop
//

#include "CommandFactory.hpp"

std::shared_ptr<ClientCommand> CommandFactory::createCommand(const std::string &data) {
    parsableFromJSON parser;
    auto command = static_cast<commandType>(parser.parseFromJSON(data));
    switch (command) {
        case INIT_REQUEST: {
            controllerType controller = BASE;
            std::shared_ptr<InitRequest> init(new InitRequest(data, controller, command));
            return init;
        }
        case CONNECT_REQUEST: {
            controllerType controller = BASE;
            std::shared_ptr<ConnectRequest> connect(new ConnectRequest(data, controller, command));
            return connect;
        }
        case START_GAME_REQUEST: {
            controllerType controller = GAME;
            std::shared_ptr<StartGameSessionRequest> start_game(
                    new StartGameSessionRequest(data, controller, command));
            return start_game;
        }
        case TEXT_REQUEST: {
            controllerType controller = GAME;
            std::shared_ptr<GetText> get_text(new GetText(data,
                                                                controller, command));
            return get_text;
        }
        case ROOM_STATUS_REQUEST: {
            controllerType controller = GAME;
            std::shared_ptr<GetRoomStatus> get_status(new GetRoomStatus(data, controller, command));
            return get_status;
        }
        case WRITTEN_TEXT_REQUEST: {
            controllerType controller = GAME;
            std::shared_ptr<SendWrittenText> written_text(new SendWrittenText(data, controller, command));
            return written_text;
        }
        default:
            return nullptr;
    }
}
