#pragma once

#include "castle/network/packet_send.hpp"
#include "castle/network/tcp_connection.hpp"
#include "castleProtocol/package/writePacket.hpp"
#include "castleProtocol/protocol/configuration.hpp"
#include "castleProtocol/protocol/play.hpp"
#include "player/player.hpp"
#include "player/postion.hpp"
#include "protocol/protocol.hpp"
#include "protocol/state.hpp"
#include "protocol/types/game_profile.hpp"
#include <chrono>
#include <cstdint>

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
      auto now = std::chrono::steady_clock::now();

      auto duration = now.time_since_epoch();

      std::int64_t milliseconds =
          std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

      if (lastPing + 10000 >= milliseconds) {
        return;
      }

      switch (_state) {
      case minecraft::protocol::State::Play: {
        castle::protocol::PacketWriter<castle::protocol::PlayClientbound> packetP(
            castle::protocol::PlayClientbound::KeepAlive);

        packetP.addInt<std::int64_t>(milliseconds);

        castle::network::sendPacket(_connection, packetP);
        break;
      }
      case minecraft::protocol::State::Configuration: {
        castle::protocol::PacketWriter<castle::protocol::ConfigurationClientbound> packetC(
            castle::protocol::ConfigurationClientbound::KeepAlive);
        packetC.addInt<std::int64_t>(milliseconds);

        castle::network::sendPacket(_connection, packetC);

      } break;
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
