//
// Created by pyxiion on 21.10.23.
//

#ifndef ENGINE_BGFX_UTILS_HPP
#define ENGINE_BGFX_UTILS_HPP
#include <string>
#include <bgfx/bgfx.h>

namespace px::utils::bgfx {
  extern const ::bgfx::Memory *stringToMemory(const std::string &str);
}

#endif //ENGINE_BGFX_UTILS_HPP
