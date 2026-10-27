#include "io/nbt_loader.hpp"
#include "castle/reader.hpp"

namespace castle::io {
  void appendsNbtFile(const std::filesystem::path& path, std::vector<std::uint8_t>& buffer) {
    castle::nbt::NbtReader::reader(path, buffer, {.namelessRoot = true});
  }
} // namespace castle::io
