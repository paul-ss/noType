//
// Created by paul_s on 17.04.2020.
//

#include "CommandFactory.hpp"
#include "Logger.hpp"



std::shared_ptr<ClientCommand> CommandFactory::createCommand(
                            const std::string &connectionUUID,
                            const std::string &data) {
  try {
    return createCommandInternal(connectionUUID, data);

  } catch (const pt::ptree_error &exc) {
    //std::cout << "CommandFactory exception: " + std::string(exc.what()) << std::endl;
    BOOST_LOG_TRIVIAL(error) << "CommandFactory exception: " + std::string(exc.what());
    return std::make_shared<ErrorRequest>(connectionUUID,
        "CommandFactory exception: " + std::string(exc.what()));
    
  } catch (std::runtime_error &exc) {
    //std::cout << "CommandFactory exception: " + std::string(exc.what()) << std::endl;
    BOOST_LOG_TRIVIAL(error) << "CommandFactory exception: " + std::string(exc.what());
    return std::make_shared<ErrorRequest>(connectionUUID,
        "CommandFactory exception: " +  std::string(exc.what()));
    
  } catch (...) {
    //std::cout << "CommandFactory : unknown exception" << std::endl;
    BOOST_LOG_TRIVIAL(error) << "CommandFactory : unknown exception";
    return std::make_shared<ErrorRequest>(connectionUUID,
        "CommandFactory : unknown exception");
    
  }
}






std::shared_ptr<ClientCommand> CommandFactory::createCommandInternal(
                            const std::string &connectionUUID,
                            const std::string &data) {
  pt::ptree tree;
  std::stringstream stream(data);
  pt::read_json(stream, tree);

  auto commandTypeStr = tree.get<std::string>(COMMAND_TYPE_JSON_PATH);
  CommandType commandType = CommandType::_from_string(commandTypeStr.c_str());


  switch (commandType) {
    case CommandType::InitRequest :
      return std::make_shared<InitRequest>(connectionUUID, std::move(tree));

    case CommandType::ConnectRequest :
      return std::make_shared<ConnectRequest>(connectionUUID, std::move(tree));

    case CommandType::StartGameSessionRequest :
      return std::make_shared<StartGameSessionRequest>(connectionUUID, std::move(tree));

    case CommandType::GetTextRequest :
      return std::make_shared<GetTextRequest>(connectionUUID, std::move(tree));

    case CommandType::RoomStatusRequest :
      return std::make_shared<RoomStatusRequest>(connectionUUID, std::move(tree));

    case CommandType::ValidateWrittenTextRequest :
      return std::make_shared<ValidateWrittenTextRequest>(connectionUUID, std::move(tree));

    default:
      return std::make_shared<ErrorRequest>(connectionUUID, "CommandFactory error: invalid command type");
  }
}
