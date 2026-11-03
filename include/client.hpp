#pragma once
#include "player.hpp"
#include "protocol/protocol.hpp"
#include "protocol/state.hpp"
#include "protocol/types/game_profile.hpp"
#include "tcp.hpp"
#include <cstdint>

class Client {
  public:
    Client(int socketId, int32_t id) : _player(id), _connection(socketId) {
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
      return _player.entityId();
    }
    Player getPlayer() const {
      return _player;
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
    Player _player;
    TcpConnection _connection;
    bool _connected = true;
    minecraft::protocol::State _state = minecraft::protocol::State::Handshake;
};
