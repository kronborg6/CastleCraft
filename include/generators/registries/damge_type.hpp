#pragma once

#include "registry/damge_types.hpp"
namespace castle::generators::registries {

  void createDamgeType(const registry::DamageTypeData& damgeType);
  void generateDamgeType(bool override = false);
} // namespace castle::generators::registries
