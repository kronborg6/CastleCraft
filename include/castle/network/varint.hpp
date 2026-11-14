#pragma once

#include "./tcp_connection.hpp"
#include <cstdint>
namespace castle::network {
std::int32_t readVarInt(castle::network::TcpConnection &connection);
} // namespace castle::network
