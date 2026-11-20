#pragma once

#include "castle/client.hpp"
#include "castle/server/server.hpp"
#include "castleProtocol/types/postion.hpp"
#include <cstdint>

namespace castle::gameplay {

  struct BlockPlacementRequest {
      type::Postion clickedPosition;
      std::int32_t face;
      std::int32_t hand;
  };

  bool placeBlock(server::Server& server, Client& client, const BlockPlacementRequest& request);

} // namespace castle::gameplay
