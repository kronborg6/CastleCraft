#include "generators/registries/dimension.hpp"
#include <castle/nbt.hpp>
#include <castle/writer.hpp>
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;

namespace castle::generators::registries {

  nbt::Nbt createOverworld() {

    auto root = castle::nbt::Nbt::makeCompound("");

    root.add(castle::nbt::Nbt::makeFloat("ambient_light", 0.0f));

    root.add(castle::nbt::Nbt::makeCompound(
        "attributes",
        {castle::nbt::Nbt::makeCompound(
             "minecraft:audio/ambient_sounds",
             {castle::nbt::Nbt::makeCompound(
                 "mood",
                 {castle::nbt::Nbt::makeInt("block_search_extent", 8),
                  castle::nbt::Nbt::makeDouble("offset", 2.0),
                  castle::nbt::Nbt::makeString("sound", "minecraft:ambient.cave"),
                  castle::nbt::Nbt::makeInt("tick_delay", 6000)})}),

         castle::nbt::Nbt::makeCompound(
             "minecraft:audio/background_music",
             {castle::nbt::Nbt::makeCompound(
                  "creative",
                  {castle::nbt::Nbt::makeInt("max_delay", 24000),
                   castle::nbt::Nbt::makeInt("min_delay", 12000),
                   castle::nbt::Nbt::makeString("sound", "minecraft:music.creative")}),

              castle::nbt::Nbt::makeCompound(
                  "default",
                  {castle::nbt::Nbt::makeInt("max_delay", 24000),
                   castle::nbt::Nbt::makeInt("min_delay", 12000),
                   castle::nbt::Nbt::makeString("sound", "minecraft:music.game")})}),

         castle::nbt::Nbt::makeString("minecraft:visual/ambient_light_color", "#0a0a0a"),

         castle::nbt::Nbt::makeString("minecraft:visual/cloud_color", "#ccffffff"),

         castle::nbt::Nbt::makeFloat("minecraft:visual/cloud_height", 192.33f),

         castle::nbt::Nbt::makeString("minecraft:visual/fog_color", "#c0d8ff"),

         castle::nbt::Nbt::makeString("minecraft:visual/sky_color", "#78a7ff")}));

    root.add(castle::nbt::Nbt::makeDouble("coordinate_scale", 1.0));

    root.add(castle::nbt::Nbt::makeString("default_clock", "minecraft:overworld"));

    root.add(castle::nbt::Nbt::makeByte("has_ceiling", 0));

    root.add(castle::nbt::Nbt::makeByte("has_ender_dragon_fight", 0));

    root.add(castle::nbt::Nbt::makeByte("has_skylight", 1));

    root.add(castle::nbt::Nbt::makeInt("height", 384));

    // root.add(castle::nbt::Nbt::makeString("infiniburn", "#minecraft:infiniburn_overworld"));

    root.add(castle::nbt::Nbt::makeInt("logical_height", 384));

    root.add(castle::nbt::Nbt::makeInt("min_y", -64));

    root.add(castle::nbt::Nbt::makeInt("monster_spawn_block_light_limit", 0));

    root.add(
        castle::nbt::Nbt::makeCompound("monster_spawn_light_level",
                                       {castle::nbt::Nbt::makeString("type", "minecraft:uniform"),

                                        castle::nbt::Nbt::makeInt("max_inclusive", 7),

                                        castle::nbt::Nbt::makeInt("min_inclusive", 0)}));
    root.add(
        castle::nbt::Nbt::makeList("infiniburn",
                                   {castle::nbt::Nbt::makeString("", "minecraft:magma_block"),
                                    castle::nbt::Nbt::makeString("", "minecraft:netherrack")}));
    root.add(castle::nbt::Nbt::makeString("timelines", "#minecraft:in_overworld"));

    return root;
  }

  void generateOverworld(bool override) {
    const fs::path path = "./generated/dimensions/overworld.nbt";

    if (std::filesystem::exists(path) && !override) {
      return;
    }

    fs::create_directories(path.parent_path());

    nbt::NbtWriter::writer(createOverworld(), path, {.namelessRoot = true});
  }

} // namespace castle::generators::registries
