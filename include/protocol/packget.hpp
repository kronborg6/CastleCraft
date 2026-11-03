#pragma once
#include "tcp.hpp"
#include <cstdint>
#include <vector>
;
namespace minecraft::protocol {
  struct Packget {
      std::int32_t packetId;
      std::vector<std::uint8_t> data;
  };

  Packget createPacket(TcpConnection& connection);
} // namespace minecraft::protocol
