#pragma once

#include "castle/network/tcp_connection.hpp"
#include "castleProtocol/protocol/configuration.hpp"
#include "castleProtocol/protocol/play.hpp"
#include "player/player.hpp"
#include "player/postion.hpp"
#include "protocol/configuration.hpp"
#include "protocol/endian.hpp"
#include "protocol/protocol.hpp"
#include "protocol/state.hpp"
#include "protocol/types/game_profile.hpp"
#include <chrono>
#include <cstdint>
#include <vector>

class Client {
  public:
    Client(int socketId, int32_t id) : _player(id), _connection(socketId) {
    }
    csatle::network::TcpConnection& connection() {
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

    Postion getPostion() const {
      return _postion;
    }

    void connectionCheck() {
      std::vector<std::uint8_t> data;

      auto now = std::chrono::steady_clock::now();

      auto duration = now.time_since_epoch();

      std::int64_t milliseconds =
          std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

      if (lastPing + 10000 >= milliseconds) {
        return;
      }

      minecraft::protocol::endian::writeBigEndian<std::int64_t>(milliseconds, data);

      switch (_state) {
      case minecraft::protocol::State::Play:
        minecraft::protocol::sendPacket(
            _connection, castle::protocol::PlayClientbound::KeepAlive, data);
        break;
      case minecraft::protocol::State::Configuration:
        minecraft::protocol::sendPacket(
            _connection, minecraft::protocol::ConfigurationClientbound::KeepAlive, data);
        break;
      default:
        return;
      }
      lastPing = milliseconds;
    }

    long lastPing;

  private:
    Player _player;
    csatle::network::TcpConnection _connection;
    Postion _postion;
    bool _connected = true;

    minecraft::protocol::State _state = minecraft::protocol::State::Handshake;
};
