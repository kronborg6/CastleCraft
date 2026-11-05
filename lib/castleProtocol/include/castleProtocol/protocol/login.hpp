#pragma once

#include <cstdint>
namespace minecraft::protocol : std::int32_t {
  enum class LoginClientbound {
    Disconnect = 0x00,
    EncryptionRequest = 0x01,
    LoginSuccess = 0x02,
    SetCompression = 0x03,
    LoginPluginRequest = 0x04,
    CookieRequest = 0x05
  };
  enum class LoginServerbound {
    LoginStart = 0x00,
    EncryptionResponse = 0x01,
    LoginPluginResponse = 0x02,
    LoginAcknowledged = 0x03,
    CookieResponse = 0x04
  };
} // namespace minecraft::protocol
