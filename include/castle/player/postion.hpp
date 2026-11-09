#pragma once

namespace castle::player {
  struct Postion {
      double x;
      double y;
      double z;

      double velocityX;
      double velocityY;
      double velocityZ;

      float yaw;
      float pitch;
  };

} // namespace castle::player
