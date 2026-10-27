#include "generators/registries/timeline.hpp"
#include "castle/nbt.hpp"
#include <castle/writer.hpp>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

namespace castle::generators::registries {
  void generateDay(const std::filesystem::path& path) {

    fs::create_directories(path.parent_path());

    auto day = castle::nbt::Nbt::makeCompound(
        "",
        {castle::nbt::Nbt::makeString("clock", "minecraft:overworld"),

         castle::nbt::Nbt::makeInt("period_ticks", 24000),

         castle::nbt::Nbt::makeCompound("tracks", {})});

    castle::nbt::NbtWriter::writer(day, path, {.namelessRoot = true});
  }
  void generateEarlyGame(const std::filesystem::path& path) {
    auto earlyGame = castle::nbt::Nbt::makeCompound(
        "",
        {castle::nbt::Nbt::makeString("clock", "minecraft:overworld"),

         castle::nbt::Nbt::makeCompound(
             "tracks",
             {castle::nbt::Nbt::makeCompound(
                 "minecraft:gameplay/can_pillager_patrol_spawn",
                 {castle::nbt::Nbt::makeList(
                      "keyframes",
                      {castle::nbt::Nbt::makeCompound("",
                                                      {castle::nbt::Nbt::makeInt("ticks", 0),
                                                       castle::nbt::Nbt::makeByte("value", 0)}),

                       castle::nbt::Nbt::makeCompound("",
                                                      {castle::nbt::Nbt::makeInt("ticks", 120000),
                                                       castle::nbt::Nbt::makeByte("value", 1)})}),

                  castle::nbt::Nbt::makeString("modifier", "and")})})});

    castle::nbt::NbtWriter::writer(earlyGame, path, {.namelessRoot = true});
  }
  void generateMoon(const std::filesystem::path& path) {
    auto moon = castle::nbt::Nbt::makeCompound(
        "",
        {castle::nbt::Nbt::makeString("clock", "minecraft:overworld"),

         castle::nbt::Nbt::makeInt("period_ticks", 192000),

         castle::nbt::Nbt::makeCompound(
             "tracks",
             {castle::nbt::Nbt::makeCompound(
                  "minecraft:gameplay/surface_slime_spawn_chance",
                  {castle::nbt::Nbt::makeString("ease", "constant"),

                   castle::nbt::Nbt::makeList(
                       "keyframes",
                       {castle::nbt::Nbt::makeCompound(
                            "",
                            {castle::nbt::Nbt::makeInt("ticks", 0),
                             castle::nbt::Nbt::makeDouble("value", 0.5)}),
                        castle::nbt::Nbt::makeCompound(
                            "",
                            {castle::nbt::Nbt::makeInt("ticks", 24000),
                             castle::nbt::Nbt::makeDouble("value", 0.375)}),
                        castle::nbt::Nbt::makeCompound(
                            "",
                            {castle::nbt::Nbt::makeInt("ticks", 48000),
                             castle::nbt::Nbt::makeDouble("value", 0.25)}),
                        castle::nbt::Nbt::makeCompound(
                            "",
                            {castle::nbt::Nbt::makeInt("ticks", 72000),
                             castle::nbt::Nbt::makeDouble("value", 0.125)}),
                        castle::nbt::Nbt::makeCompound(
                            "",
                            {castle::nbt::Nbt::makeInt("ticks", 96000),
                             castle::nbt::Nbt::makeDouble("value", 0.0)}),
                        castle::nbt::Nbt::makeCompound(
                            "",
                            {castle::nbt::Nbt::makeInt("ticks", 120000),
                             castle::nbt::Nbt::makeDouble("value", 0.125)}),
                        castle::nbt::Nbt::makeCompound(
                            "",
                            {castle::nbt::Nbt::makeInt("ticks", 144000),
                             castle::nbt::Nbt::makeDouble("value", 0.25)}),
                        castle::nbt::Nbt::makeCompound(
                            "",
                            {castle::nbt::Nbt::makeInt("ticks", 168000),
                             castle::nbt::Nbt::makeDouble("value", 0.375)})}),

                   castle::nbt::Nbt::makeString("modifier", "maximum")}),

              castle::nbt::Nbt::makeCompound(
                  "minecraft:visual/moon_phase",
                  {castle::nbt::Nbt::makeList(
                      "keyframes",
                      {castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 0),
                            castle::nbt::Nbt::makeString("value", "full_moon")}),
                       castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 24000),
                            castle::nbt::Nbt::makeString("value", "waning_gibbous")}),
                       castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 48000),
                            castle::nbt::Nbt::makeString("value", "third_quarter")}),
                       castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 72000),
                            castle::nbt::Nbt::makeString("value", "waning_crescent")}),
                       castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 96000),
                            castle::nbt::Nbt::makeString("value", "new_moon")}),
                       castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 120000),
                            castle::nbt::Nbt::makeString("value", "waxing_crescent")}),
                       castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 144000),
                            castle::nbt::Nbt::makeString("value", "first_quarter")}),
                       castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 168000),
                            castle::nbt::Nbt::makeString("value", "waxing_gibbous")})})})})});

    castle::nbt::NbtWriter::writer(moon, path, {.namelessRoot = true});
  }
  void generateVillagerSchedule(const std::filesystem::path& path) {
    auto villagerSchedule = castle::nbt::Nbt::makeCompound(
        "",
        {castle::nbt::Nbt::makeString("clock", "minecraft:overworld"),

         castle::nbt::Nbt::makeInt("period_ticks", 24000),

         castle::nbt::Nbt::makeCompound(
             "tracks",
             {castle::nbt::Nbt::makeCompound(
                  "minecraft:gameplay/baby_villager_activity",
                  {castle::nbt::Nbt::makeList(
                      "keyframes",
                      {castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 10),
                            castle::nbt::Nbt::makeString("value", "minecraft:idle")}),
                       castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 3000),
                            castle::nbt::Nbt::makeString("value", "minecraft:play")}),
                       castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 6000),
                            castle::nbt::Nbt::makeString("value", "minecraft:idle")}),
                       castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 10000),
                            castle::nbt::Nbt::makeString("value", "minecraft:play")}),
                       castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 12000),
                            castle::nbt::Nbt::makeString("value", "minecraft:rest")})})}),

              castle::nbt::Nbt::makeCompound(
                  "minecraft:gameplay/villager_activity",
                  {castle::nbt::Nbt::makeList(
                      "keyframes",
                      {castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 10),
                            castle::nbt::Nbt::makeString("value", "minecraft:idle")}),
                       castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 2000),
                            castle::nbt::Nbt::makeString("value", "minecraft:work")}),
                       castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 9000),
                            castle::nbt::Nbt::makeString("value", "minecraft:meet")}),
                       castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 11000),
                            castle::nbt::Nbt::makeString("value", "minecraft:idle")}),
                       castle::nbt::Nbt::makeCompound(
                           "",
                           {castle::nbt::Nbt::makeInt("ticks", 12000),
                            castle::nbt::Nbt::makeString("value", "minecraft:rest")})})})})});

    castle::nbt::NbtWriter::writer(villagerSchedule, path, {.namelessRoot = true});
  }

  void generateTimelines(bool override) {
    const std::filesystem::path directory = "./generated/registries/timeline";

    std::filesystem::create_directories(directory);

    if (!std::filesystem::exists(directory / "day.nbt") || override) {
      generateDay(directory / "day.nbt");
    }

    if (!std::filesystem::exists(directory / "early_game.nbt") || override) {
      generateEarlyGame(directory / "early_game.nbt");
    }

    if (!std::filesystem::exists(directory / "moon.nbt") || override) {
      generateMoon(directory / "moon.nbt");
    }

    if (!std::filesystem::exists(directory / "villager_schedule.nbt") || override) {
      generateVillagerSchedule(directory / "villager_schedule.nbt");
    }
  }
} // namespace castle::generators::registries
