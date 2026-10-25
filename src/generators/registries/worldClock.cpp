
#include "castle/nbt.hpp"
#include <castle/writer.hpp>
#include <filesystem>

namespace fs = std::filesystem;

namespace castle::generators::registries {
  castle::nbt::Nbt createWorldClock() {

    auto clock = castle::nbt::Nbt::makeCompound("");

    return clock;
  }

  void generateWorldClock() {

    const fs::path path = "./generated/registries/dimension/worldClock.nbt";

    fs::create_directories(path.parent_path());
    auto clock = nbt::Nbt::makeCompound("");

    nbt::NbtWriter::writer(
        clock, "./generated/registries/dimension/overworld.nbt", {.namelessRoot = true});
  }
} // namespace castle::generators::registries
