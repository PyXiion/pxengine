//
// Created by pyxiion on 22.10.23.
//

#ifndef PX_SYSTEM_HPP
#define PX_SYSTEM_HPP
#include <algorithm>
#include <concepts>
#include <bit>
#include "exports.hpp"

#ifdef __GNUC__
  #define PX_GCC
#endif
#ifdef __clang__
  #define PX_CLANG
#endif

namespace px {
  template <class T>
  concept Integer = std::is_integral_v<T>;

  template <Integer T>
  PX_API constexpr inline T convertEndianness(T value) {
    constexpr std::size_t size = sizeof(T);

    if constexpr (size == 1) {
      return value;
    } else {
      std::reverse(reinterpret_cast<std::byte*>(&value),
                   reinterpret_cast<std::byte*>(&value) + size);
      return value;
    }
  }

  template <Integer T>
  PX_API constexpr inline T toNetworkEndianness(T value) {
    if constexpr (std::endian::native == std::endian::little) {
      return value;
    } else if constexpr (std::endian::native == std::endian::big) {
      return convertEndianness(value);
    } else {
      return 0;
    }
  }

  template <Integer T>
  PX_API constexpr inline T toHostEndianness(T value) {
    if constexpr (std::endian::native == std::endian::big) {
      return value;
    } else if constexpr (std::endian::native == std::endian::little) {
      return convertEndianness(value);
    } else {
      return 0;
    }
  }
}

#endif //PX_SYSTEM_HPP
