#pragma once

#include <cstdint>
#include <string>
#include <vector>
namespace minecraft::nbt {
void ToTagShort(std::string name, int16_t value,
                std::vector<std::uint8_t> &data);
} // namespace minecraft::nbt
