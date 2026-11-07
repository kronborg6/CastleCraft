#pragma once

#include "./tcp_connection.hpp"
#include <cstdint>
#include <vector>
namespace castle::network {
[[nodiscard("do you not need client data???")]]
std::vector<std::uint8_t> receivePacket(csatle::network::TcpConnection& connection);
}
