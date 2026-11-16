#pragma once

#include "castle/client.hpp"
#include "castle/network/tcp_connection.hpp"
#include "castle/server/server.hpp"
#include "castle/world/chunk.hpp"
#include "castle/world/chunkPostion.hpp"
#include <cstdint>
namespace castle::handlers::play {
  void startChunkBatch(castle::network::TcpConnection& connection);

  void finishChunkBatch(castle::network::TcpConnection& connection, std::int32_t count);

  void updatePlayerLoadeChunks(server::Server& server, Client& client);

  void unloadPlayerChunk(castle::network::TcpConnection& connection,
                         const world::ChunkPosition& postion);

  void setPlayerCenterChunk(castle::network::TcpConnection& connection,
                            const world::ChunkPosition& postion);

  void sendStoneChunk(castle::network::TcpConnection& connection,
                      const world::ChunkPosition& postion);

  void loadChunk(Client& client, const world::Chunk& chunk);
} // namespace castle::handlers::play
