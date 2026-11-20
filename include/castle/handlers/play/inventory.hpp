#pragma once

#include "castle/client.hpp"
#include "castle/item/item.hpp"
#include <cstdint>
namespace castle::handlers::play {
  void updateInventory(Client& client, std::uint16_t slot, item::Item& item);
} // namespace castle::handlers::play
