#pragma once

#include "castle/client.hpp"
#include "castle/network/tcp_connection.hpp"
#include "castle/world/chunk.hpp"
#include "castle/world/chunkPostion.hpp"
namespace castle::handlers::play {
  void startChunkBatch(csatle::network::TcpConnection& connection);
  void finishChunkBatch(csatle::network::TcpConnection& connection);

  void updatePlayerLoadeChunks(Client& client);
  void unloadPlayerChunk(csatle::network::TcpConnection& connection,
                         const world::ChunkPosition& postion);
  void setPlayerCenterChunk(csatle::network::TcpConnection& connection,
                            world::ChunkPosition postion);

  void sendStoneChunk(csatle::network::TcpConnection& connection,
                      const world::ChunkPosition& postion);
  void loadChunk(Client& client, const world::Chunk& chunk);
} // namespace castle::handlers::play
