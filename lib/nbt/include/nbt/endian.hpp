#pragma once
#include <cstdint>
#include <vector>

namespace endian {
template <typename T>
void writeBigEndian(T value, std::vector<std::uint8_t> &data);
template <typename T>
void writeLittleEndian(T value, std::vector<std::uint8_t> &data);
} // namespace endian
