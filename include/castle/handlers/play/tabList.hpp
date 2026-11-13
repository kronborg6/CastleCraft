#pragma once

#include "castle/client.hpp"
#include "castle/handlers/play/play.hpp"
namespace castle::handlers::play {
  void playerInfoUpdate(Client& client, player::PlayerAction action);

}
