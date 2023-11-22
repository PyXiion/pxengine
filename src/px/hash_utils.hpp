// Copyright (c) 2023.

//
// Created by pyxiion on 05.11.23.
//

#ifndef ENGINE_HASH_UTILS_HPP
#define ENGINE_HASH_UTILS_HPP

#include <cstddef>
#include <functional>

namespace px {
  template <class T>
  constexpr inline void combine_hash(std::size_t &hash, const T &v) {
    std::hash<T> hasher;
    hash ^= hasher(v) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
  }

  struct pair_hash {
    template <class T1, class T2>
    std::size_t operator () (const std::pair<T1,T2> &p) const {
      auto h1 = std::hash<T1>{}(p.first);
      combine_hash(h1, p.second);
      return h1;
    }
  };
}

#endif //ENGINE_HASH_UTILS_HPP
