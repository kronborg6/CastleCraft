#pragma once

#include <cstdint>
namespace castle::protocol {
  enum class ConfigurationClientbound : std::int32_t {
    CookieRequest = 0x00,
    PluginMessage = 0x01,
    Disconnect = 0x02,
    FinishConfiguration = 0x03,
    KeepAlive = 0x04,
    Ping = 0x05,
    ResetChat = 0x06,
    RegistryData = 0x07,
    RemoveResourcePack = 0x08,
    AddResourcePack = 0x09,
    PostEffects = 0x0A,
    StoreCookie = 0x0B,
    Transfer = 0x0C,
    FeatureFlags = 0x0D,
    UpdateTags = 0x0E,
    KnownPacks = 0x0F,
    CustomReportDetails = 0x10,
    ServerLinks = 0x11,
    ClearDialog = 0x12,
    ShowDialog = 0x13,
    CodeOfConduct = 0x14,
  };
  enum class ConfigurationServerbound : std::int32_t {
    ClientInfomation = 0x0,
    CookieResponse = 0x01,
    PluginMessage = 0x02,
    AcknowledgeFinishConfiguration = 0x03,
    KeepAlive = 0x04,
    Pong = 0x05,
    ResourcePackResponse = 0x06,
    KnownPacks = 0x07,
    CustomClickAction = 0x08,
    AcceptCodeOfConduct = 0x09,
  };
} // namespace castle::protocol
