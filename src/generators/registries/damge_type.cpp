#include "generators/registries/damge_type.hpp"
#include "castle/nbt.hpp"
#include "castle/writer.hpp"
#include "registry/damge_types.hpp"
#include <algorithm>
#include <string>

#include <filesystem>

namespace fs = std::filesystem;

namespace castle::generators::registries {

  void createDamgeType(const registry::DamageTypeData& damgeType) {

    auto damge = nbt::Nbt::makeCompound("");

    damge.add(nbt::Nbt::makeFloat("exhaustion", damgeType.exhaustion));
    damge.add(nbt::Nbt::makeString("message_id", damgeType.messageId));
    damge.add(nbt::Nbt::makeString("scaling", damgeType.scaling));

    nbt::NbtWriter::writer(damge,
                           "./generated/registries/damge_types/" + damgeType.messageId + ".nbt",
                           {.namelessRoot = true});
  }
  void generateDamgeType(bool override) {
    const fs::path directory = "./generated/registries/damage_types";
    for (auto damgeType : registry::DAMAGE_TYPES) {
      const fs::path path = directory / (damgeType.messageId + ".nbt");
      if (std::filesystem::exists(path) && !override) {
        continue;
      }

      createDamgeType(damgeType);
    }
  }
} // namespace castle::generators::registries
