#include "generators/registries/worldClock.hpp"
#include "castle/nbt.hpp"
#include <castle/writer.hpp>
#include <filesystem>

namespace fs = std::filesystem;

namespace castle::generators::registries {
  castle::nbt::Nbt createWorldClock() {

    auto clock = castle::nbt::Nbt::makeCompound("");

    return clock;
  }

  void generateWorldClock(bool override) {

    const fs::path path = "./generated/registries/dimension/worldClock.nbt";

    if (std::filesystem::exists(path) && !override) {
      return;
    }
    fs::create_directories(path.parent_path());
    auto clock = nbt::Nbt::makeCompound("");

    nbt::NbtWriter::writer(
        clock, "./generated/registries/dimension/overworld.nbt", {.namelessRoot = true});
  }
} // namespace castle::generators::registries
