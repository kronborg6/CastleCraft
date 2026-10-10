#pragma once
#include "protocol.hpp"

class Client {
  public:
    Client(int socketId) : _socketId(socketId) {
    }
    int getSocket() const {
      return _socketId;
    }
    minecraft::protocol::State getState() const {
      return _state;
    }
    void updateState(minecraft::protocol::State state) {
      _state = state;
    }

  private:
    int _socketId;
    minecraft::protocol::State _state = minecraft::protocol::State::Handshake;
};
