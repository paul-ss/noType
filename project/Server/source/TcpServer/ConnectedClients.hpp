//
// Created by paul_s on 21.04.2020.
//

#pragma once



#include <unordered_map>
#include <string>
#include <memory>
#include <exception>
#include <mutex>

class Client;

class ConnectedClients {
public:
  virtual bool insert(const std::shared_ptr<Client> &client);
  virtual bool erase(const std::string &clientUUID);
  virtual std::shared_ptr<Client> getClient(const std::string &clientUUID);
private:
  std::unordered_map<std::string, std::shared_ptr<Client>> _clients;
  std::mutex _connectedClientsMutex;
};


