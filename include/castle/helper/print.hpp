#pragma once

#include "castle/client.hpp"
#include <iostream>
namespace castle::helper::print {

  void printMovemet(castle::Client& client) {

    auto profile = client.profile;
    auto pos = client.getPlayer().getPostion();

    std::cout << "Player " << client.profile.username << " moved:\n"
              << "  X: " << pos.x << " -> " << pos.x << " (delta: " << pos.x - pos.x << ")\n"
              << "  Yaw: " << pos.yaw << "\n"
              << "  Pitch: " << pos.pitch << '\n';
  }
} // namespace castle::helper::print
