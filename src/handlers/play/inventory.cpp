#include "castle/handlers/play/inventory.hpp"
#include "castle/client.hpp"
#include "castleProtocol/types/postion.hpp"

namespace castle::handlers::play {

  void updateInventory(Client& client, std::uint16_t slot, item::Item& item) {
    auto& inv = client.getPlayer().getInventory();

    if (slot > 35) {

      inv.insert(slot - 36, item);
    } else {
      inv.insert(slot, item);
    }
  }

  bool placeBlock(Client& client, type::Postion& postion) {

    auto& inv = client.getPlayer().getInventory();

    auto item = inv.getHeldItem();

    if (item == nullptr || item->count == 0) {
      return false;
    }

    return true;
  }
} // namespace castle::handlers::play
