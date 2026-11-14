#include "castle/handlers/play/join.hpp"
#include "castle/handlers/play/chunk.hpp"
#include "castle/handlers/play/gameEvent.hpp"
#include "castle/handlers/play/movment.hpp"
#include "castle/handlers/play/play.hpp"
#include "castle/handlers/play/tabList.hpp"
#include "castle/network/packet_send.hpp"
#include "castle/player/player.hpp"
#include "castleProtocol/protocol/state.hpp"

namespace castle::handlers::play {

  void start(castle::network::TcpConnection& connection) {

    castle::protocol::PacketWriter<castle::protocol::PlayClientbound> packet(
        castle::protocol::PlayClientbound::GameEvent);

    packet.addByte(0xd);
    packet.addFloat(0.0f);

    castle::network::sendPacket(connection, packet);
  }
  void join(server::Server& server, Client& client) {
    castle::network::TcpConnection& connection = client.connection();

    PlayPacketWriter play(castle::protocol::PlayClientbound::Login);

    // Player Enitity ID
    play.addInt<std::int32_t>(client.getEntityId());
    // Is Hardcore
    play.addByte(0x00);
    // Dimension Names
    std::vector<std::string> dimensions = {"minecraft:overworld"};
    play.addVarInt(static_cast<std::int32_t>(dimensions.size()));
    for (const auto& dimension : dimensions) {
      play.addString(dimension);
    }
    // Max Players
    play.addVarInt(server.getMaxPlayerCount());
    // View Distance
    play.addVarInt(player::VIEW);
    // Simulation Distance
    play.addVarInt(player::VIEW);
    // Reduced Debug Info
    play.addBoolean(false);
    // Enable respawn screen
    play.addBoolean(true);
    // Do limited crafting
    play.addBoolean(false);
    // Dimension Type
    play.addVarInt(0);
    // Dimension Name
    play.addString("minecraft:overworld");
    // Hashed seed (not used at the moment just wanna join)
    play.addInt<std::int64_t>(std::uint64_t{0});
    // Game mode
    play.addVarInt(1);
    // Previous Game mode
    play.addVarInt(0);
    // is Debug (flase)
    play.addBoolean(false);
    // Is Flat (false)
    play.addBoolean(false);
    // Has death location
    play.addBoolean(false);
    // Portal cooldown
    play.addVarInt(1000);
    // Sea Level
    play.addVarInt(20);
    // Online mode
    play.addBoolean(true);
    // Enforces Secure Chat
    play.addBoolean(false);

    castle::network::sendPacket(connection, play);

    gameEvent(connection, GameEvent::StartWaitingForChunks, 0.0f);

    teleportPlayer(client, {0, 64, 0, 0, 0, 0});

    setPlayerCenterChunk(connection, {0, 0});
    startChunkBatch(connection);

    std::int32_t count = 0;
    for (int i = -2; i <= 2; i++) {
      for (int x = -2; x <= 2; x++) {
        count++;
        sendStoneChunk(connection, {i, x});
        // chuck(connection, i, x);
      }
    }

    finishChunkBatch(connection, count);

    // auto thisPlayer = playerInfoUpdate(client, player::PlayerAction::AddPlayer);
    //
    // client.broadcast(thisPlayer, true);

    auto players = server.getClientsByStatus(protocol::State::Play);

    for (const auto& player : players) {
      auto playerPacket = playerInfoUpdate(*player, player::PlayerAction::AddPlayer);
      network::sendPacket(connection, playerPacket);
    }

    // playerInfoUpdate(clinet, player::PlayerAction::AddPlayer);
  }
  void leave(server::Server&, Client& client);
} // namespace castle::handlers::play
