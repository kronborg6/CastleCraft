#pragma once

#include <cstdint>
#include <filesystem>
#include <vector>
namespace castle::io {
  void appendsNbtFile(const std::filesystem::path& path, std::vector<std::uint8_t>& buffer);

} // namespace castle::io
