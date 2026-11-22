#include "castle/gameplay/blockPlacement.hpp"
#include "castle/client.hpp"
#include "castle/server/server.hpp"
#include "castleProtocol/types/postion.hpp"

namespace castle::gameplay {
  bool placeBlock(server::Server& server, Client& client, const type::Postion& postion) {

    auto& player = client.getPlayer();
    auto& inventory = player.getInventory();

    auto* item = inventory.getHeldItem();

    if (item == nullptr || item->count >= 0) {
      return false;
    }

    // check if client can place block
    // make a function call that triggers all plugins
    //
    // if (!canPlaceBlock(player, block, request.postion))
    //

    // later when we have more then one world get current world from client
    server.setBlock(postion, 1);

    return false;
  }
} // namespace castle::gameplay
