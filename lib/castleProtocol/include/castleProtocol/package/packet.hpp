#pragma once

#include <type_traits>
namespace castle::protocol {
  template <typename T>
  concept Integral = std::is_integral_v<T>;

  template <typename T>
  concept Enum = std::is_enum_v<T>;

} // namespace castle::protocol
