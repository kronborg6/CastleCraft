#include "endian.hpp"
#include <bit>
#include <cstdint>
#include <type_traits>
#include <vector>
namespace castle::endian {

  uint32_t floatToUint32(float value) {

    uint32_t reslut = std::bit_cast<uint32_t>(value);
    return reslut;
  }

  int32_t floatToint32(float value) {
    int32_t reslut = std::bit_cast<int32_t>(value);
    return reslut;
  }
  int64_t doubleToint64(double value) {
    int64_t reslut = std::bit_cast<int64_t>(value);
    return reslut;
  }

  uint64_t doubleToUint64(double value) {
    uint64_t reslut = std::bit_cast<uint64_t>(value);
    return reslut;
  }

  int64_t longToInt64(long value) {
    int64_t result = std::bit_cast<int64_t>(value);
    return result;
  }

  float uint32ToFloat(uint32_t from) {
    float result = std::bit_cast<float>(from);

    return result;
  }
  double uint64ToDouble(uint64_t from) {
    double reslut = std::bit_cast<double>(from);

    return reslut;
  }

  long int64ToLong(int64_t from) {
    long result = std::bit_cast<long>(from);
    return result;
  }
} // namespace castle::endian
