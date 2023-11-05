//
// Created by pyxiion on 21.10.23.
//

#include "bgfx_utils.hpp"

namespace px::utils::bgfx {

  const ::bgfx::Memory *stringToMemory(const std::string &str) {
    return ::bgfx::copy(str.data(), str.size() * sizeof(str[0]));
  }
}