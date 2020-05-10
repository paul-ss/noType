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
            std::shared_ptr<InitRequest> init(new InitRequest(data, command, controller));
            return init;
        }
        case CONNECT_REQUEST: {
            controllerType controller = BASE;
            std::shared_ptr<ConnectRequest> connect(new ConnectRequest(data, command, controller));
            return connect;
        }
        case START_GAME_SESSION_REQUEST: {
            controllerType controller = GAME;
            std::shared_ptr<StartGameSessionRequest> start_game(
                    new StartGameSessionRequest(data, command, controller));
            return start_game;
        }
        case GET_TEXT_REQUEST: {
            controllerType controller = GAME;
            std::shared_ptr<GetTextRequest> get_text(new GetTextRequest(data,
                                                                        command, controller));
            return get_text;
        }
        case ROOM_STATUS_REQUEST: {
            controllerType controller = GAME;
            std::shared_ptr<RoomStatusRequest> get_status(new RoomStatusRequest(data, command, controller));
            return get_status;
        }
        case VALIDATE_WRITTEN_TEXT_REQUEST: {
            controllerType controller = GAME;
            std::shared_ptr<ValidateWrittenTextRequest> written_text(new ValidateWrittenTextRequest(data,
                                                                            command, controller));
            return written_text;
        }
        default:
            return nullptr;
    }
}
