#pragma once

#include "castle/client.hpp"

namespace minecraft::protocol::handlers {
void handleStatus(Client &client);
void handleStatusRequst(Client &client);
void handlePingRequst(Client &client);
} // namespace minecraft::protocol::handlers
