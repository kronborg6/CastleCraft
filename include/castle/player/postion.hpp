#pragma once

#include <cstdint>
namespace castle::player {
  enum class UpdateType {
    Position,
    Rotation,
    PositionAndRotation
  };
  struct Position {
      double x = 0.0;
      double y = 0.0;
      double z = 0.0;

      float yaw = 0.0f;
      float pitch = 0.0f;

      std::uint8_t flags = 0;
  };

} // namespace castle::player
