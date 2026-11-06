#pragma once

#include "configuration.hpp"
#include "handsake.hpp"
#include "login.hpp"
#include "status.hpp"
#include <variant>
namespace castle::protocol {
  using PackageId = std::variant<LoginClientbound,
                                 LoginServerbound,
                                 ConfigurationClientbound,
                                 ConfigurationClientbound,
                                 StatusClientbound,
                                 StatusServerbound,
                                 HandshakeServerbound>;
}
