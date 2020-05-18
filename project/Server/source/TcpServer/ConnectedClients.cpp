//
// Created by paul_s on 21.04.2020.
//

#include "ConnectedClients.hpp"
#include "Client.hpp"

bool ConnectedClients::insert(const std::shared_ptr<Client> &client) {
  std::unique_lock<std::mutex> lock(_connectedClientsMutex);
  auto pair = _clients.emplace(client->getConnectionUUID(), client);
  return pair.second;
}

bool ConnectedClients::erase(const std::string &clientUUID) {
  std::unique_lock<std::mutex> lock(_connectedClientsMutex);
  if (_clients.count(clientUUID) > 0) {
    _clients.erase(clientUUID);
    return true;
  } else {
    return false;
  }
}

std::shared_ptr<Client> ConnectedClients::getClient(const std::string &clientUUID) {
  std::unique_lock<std::mutex> lock(_connectedClientsMutex);
  if (_clients.count(clientUUID) > 0) {
    return _clients[clientUUID];
  } else {
    throw std::out_of_range("Element with key " + clientUUID + " doesn't exist");
  }
}