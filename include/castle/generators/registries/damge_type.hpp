#pragma once

#include "castle/registry/damge_types.hpp"
#include <filesystem>
namespace castle::generators::registries {

  void createDamgeType(const registry::DamageTypeData& damgeType, const std::filesystem::path& dir);
  void generateDamgeType(bool override = false);
} // namespace castle::generators::registries
