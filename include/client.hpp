#pragma once
#include "protocol/protocol.hpp"
#include "protocol/state.hpp"
#include "protocol/types/game_profile.hpp"
#include "tcp.hpp"
#include <cstdint>

class Client {
  public:
    Client(int socketId, int32_t id) : _entityId(id), _connection(socketId) {
    }
    TcpConnection& connection() {
      return _connection;
    }
    // int getSocket() const {
    //   return _connection.;
    // }
    minecraft::protocol::State getState() const {
      return _state;
    }
    void updateState(minecraft::protocol::State state) {
      _state = state;
    }
    bool isConnected() const {
      return _connected;
    }

    int32_t getEntityId() const {
      return _entityId;
    }

    void disconected() {
      _connected = false;
    }

    ~Client() {
      _connected = false;
    }

    GameProfile profile;
    bool sent = false;

  private:
    int32_t _entityId;
    TcpConnection _connection;
    bool _connected = true;
    minecraft::protocol::State _state = minecraft::protocol::State::Handshake;
};
