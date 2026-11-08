#pragma once
#include "castleProtocol/package/writePacket.hpp"
#include "castleProtocol/protocol/configuration.hpp"
#include <cstdint>
#include <vector>

namespace castle::entries {

  using ConfigurationPacket =
      castle::protocol::PacketWriter<castle::protocol::ConfigurationClientbound>;
  void bannerTagsEntrie(ConfigurationPacket& output);

} // namespace castle::entries
