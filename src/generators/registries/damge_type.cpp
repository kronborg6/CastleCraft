#include "castle/generators/registries/damge_type.hpp"
#include "castle/nbt.hpp"
#include "castle/registry/damge_types.hpp"
#include "castle/writer.hpp"
#include <string>

#include <filesystem>

namespace fs = std::filesystem;

namespace castle::generators::registries {

  void createDamgeType(const registry::DamageTypeData& damgeType, const fs::path& dir) {

    auto damge = nbt::Nbt::makeCompound("");

    damge.add(nbt::Nbt::makeFloat("exhaustion", damgeType.exhaustion));
    damge.add(nbt::Nbt::makeString("message_id", damgeType.messageId));
    damge.add(nbt::Nbt::makeString("scaling", damgeType.scaling));

    nbt::NbtWriter::writer(damge, dir / (damgeType.messageId + ".nbt"), {.namelessRoot = true});
  }
  void generateDamgeType(bool override) {
    const fs::path directory = "./generated/registries/damage_types";

    for (auto damgeType : registry::DAMAGE_TYPES) {
      const fs::path path = directory / (damgeType.messageId + ".nbt");
      if (std::filesystem::exists(path) && !override) {
        continue;
      }

      createDamgeType(damgeType, directory);
    }
  }
} // namespace castle::generators::registries
