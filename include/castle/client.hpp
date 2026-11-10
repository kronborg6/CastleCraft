#pragma once

#include "castle/network/packet_send.hpp"
#include "castle/network/tcp_connection.hpp"
#include "castle/player/game_profile.hpp"
#include "castle/player/player.hpp"
#include "castleProtocol/package/writePacket.hpp"
#include "castleProtocol/protocol/configuration.hpp"
#include "castleProtocol/protocol/play.hpp"
#include "castleProtocol/protocol/state.hpp"
#include "network/packet_send.hpp"
#include "network/tcp_connection.hpp"
#include "player/game_profile.hpp"
#include "player/player.hpp"
#include <chrono>
#include <cstdint>

namespace castle {

  class Client {
    public:
      Client(int socketId, int32_t id) : _player(id), _connection(socketId) {
      }
      csatle::network::TcpConnection& connection() {
        return _connection;
      }

      castle::protocol::State getState() const {
        return _state;
      }
      void updateState(castle::protocol::State state) {
        _state = state;
      }
      bool isConnected() const {
        return _connected;
      }

      int32_t getEntityId() const {
        return _player.entityId();
      }
      player::Player& getPlayer() {
        return _player;
      }
      const player::Player& getPlayer() const {
        return _player;
      }

      void disconected() {
        _connected = false;
      }

      ~Client() {
        _connected = false;
      }

      player::GameProfile profile;
      bool sent = false;

      // castle::player::Position getPostion() const {
      //   return _player.po;
      // }

      void connectionCheck() {
        auto now = std::chrono::steady_clock::now();

        auto duration = now.time_since_epoch();

        std::int64_t milliseconds =
            std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

        if (lastPing + 10000 >= milliseconds) {
          return;
        }

        switch (_state) {
        case castle::protocol::State::Play: {
          castle::protocol::PacketWriter<castle::protocol::PlayClientbound> packetP(
              castle::protocol::PlayClientbound::KeepAlive);

          packetP.addInt<std::int64_t>(milliseconds);

          network::sendPacket(_connection, packetP);
          break;
        }
        case castle::protocol::State::Configuration: {
          castle::protocol::PacketWriter<castle::protocol::ConfigurationClientbound> packetC(
              castle::protocol::ConfigurationClientbound::KeepAlive);
          packetC.addInt<std::int64_t>(milliseconds);

          network::sendPacket(_connection, packetC);

        } break;
        default:
          return;
        }
        lastPing = milliseconds;
      }

      std::int64_t lastPing = 0;

    private:
      player::Player _player;
      csatle::network::TcpConnection _connection;
      bool _connected = true;

      castle::protocol::State _state = castle::protocol::State::Handshake;
  };
} // namespace castle
