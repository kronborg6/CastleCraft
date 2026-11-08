#pragma once

#include "./tcp_connection.hpp"
#include <cstdint>
#include <vector>
namespace castle::network {

  inline constexpr std::int32_t MAX_PACKET_SIZE = (1 << 21) - 1;

  [[nodiscard("do you not need client data???")]]
  std::vector<std::uint8_t> receivePacket(csatle::network::TcpConnection& connection);
} // namespace castle::network
