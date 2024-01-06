//
// Created by pyxiion on 21.10.23.
//

#ifndef ENGINE_BGFX_UTILS_HPP
#define ENGINE_BGFX_UTILS_HPP
#include <string>
#include <bgfx/bgfx.h>

namespace px::utils::bgfx {
  extern const ::bgfx::Memory *stringToMemory(const std::string &str);

  inline bool checkAvailTransientBuffers(uint32_t _numVertices, const ::bgfx::VertexLayout& _layout, uint32_t _numIndices)
  {
    return _numVertices == getAvailTransientVertexBuffer(_numVertices, _layout)
           && (0 == _numIndices || _numIndices == ::bgfx::getAvailTransientIndexBuffer(_numIndices) )
        ;
  }
}

#endif //ENGINE_BGFX_UTILS_HPP
