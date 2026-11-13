#include "castle/handlers/play/chunk.hpp"
#include "castle/handlers/play/play.hpp"
#include "castle/network/packet_send.hpp"
#include "castle/network/tcp_connection.hpp"
#include "castle/world/woldGreanter.hpp"
#include "castleProtocol/protocol/play.hpp"
#include <cstdint>

namespace castle::handlers::play {

  void startChunkBatch(csatle::network::TcpConnection& connection) {

    PlayPacketWriter packet(protocol::PlayClientbound::ChunkBatchStart);

    network::sendPacket(connection, packet);
  }
  void finishChunkBatch(csatle::network::TcpConnection& connection, std::int32_t count) {

    PlayPacketWriter packet(protocol::PlayClientbound::ChunkBatchFinished);

    packet.addVarInt(count);

    network::sendPacket(connection, packet);
  }

  void sendStoneChunk(csatle::network::TcpConnection& connection,
                      const world::ChunkPosition& position) {

    world::Chunk chunk = world::generateStoneChunk(position);

    auto packet = world::encodeChunk(chunk);

    castle::network::sendPacket(connection, packet);
  }

  // TODO
  // either her or where this is called we need to add the chunk postion to the client
  //
  //
  // musch later when player are load if in range of x of player
  // either her or where it is called load player in range
  void loadChunk(Client& client, const world::Chunk& chunk) {

    csatle::network::TcpConnection& connection = client.connection();

    auto packet = world::encodeChunk(chunk);

    castle::network::sendPacket(connection, packet);
  }

  void updatePlayerLoadeChunks(Client& client) {

    auto& connection = client.connection();

    auto& player = client.getPlayer();
    const auto center = player.getChunkPostion();

    for (auto it = client.loadedChunks.begin(); it != client.loadedChunks.end();) {

      const auto& pos = *it;

      if (std::abs(pos.x - center.x) > player::VIEW || std::abs(pos.z - center.z) > player::VIEW) {

        unloadPlayerChunk(connection, pos);
        it = client.loadedChunks.erase(it);
      } else {
        ++it;
      }
    }

    setPlayerCenterChunk(connection, center);

    std::vector<world::ChunkPosition> chunksToSend;

    for (int x = -player::VIEW; x <= player::VIEW; ++x) {
      for (int z = -player::VIEW; z <= player::VIEW; ++z) {

        world::ChunkPosition position{center.x + x, center.z + z};

        if (!client.loadedChunks.contains(position)) {
          chunksToSend.push_back(position);
        }
      }
    }

    if (chunksToSend.empty()) {
      return;
    }

    startChunkBatch(connection);

    for (const auto& position : chunksToSend) {
      sendStoneChunk(connection, position);
      client.loadedChunks.insert(position);
    }

    finishChunkBatch(connection, static_cast<std::int32_t>(chunksToSend.size()));
  }
  void unloadPlayerChunk(csatle::network::TcpConnection& connection,
                         const world::ChunkPosition& postion) {

    PlayPacketWriter packet(protocol::PlayClientbound::UnloadChunk);

    packet.addInt<std::int32_t>(postion.z);
    packet.addInt<std::int32_t>(postion.x);

    network::sendPacket(connection, packet);
  }
  void setPlayerCenterChunk(csatle::network::TcpConnection& connection,
                            const world::ChunkPosition& postion) {

    PlayPacketWriter packet(castle::protocol::PlayClientbound::SetCenterChunk);

    packet.addVarInt(postion.x);
    packet.addVarInt(postion.z);

    network::sendPacket(connection, packet);
  }
} // namespace castle::handlers::play
