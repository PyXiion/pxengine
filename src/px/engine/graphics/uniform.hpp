// Copyright (c) 2023.

//
// Created by pyxiion on 07.12.23.
//

#ifndef PX_ENGINE_UNIFORM_HPP
#define PX_ENGINE_UNIFORM_HPP
#include <memory>
#include <bgfx/bgfx.h>
#include "px/engine/resources/bgfx_handle.hpp"

namespace px {

  class Uniform {
  public:
    Uniform(const std::string &name, bgfx::UniformType::Enum type);

    template<typename T>
    Uniform &operator=(const T &value);

  private:
    BgfxUniqueUniformHandle m_handle;
  };

  typedef std::shared_ptr<Uniform> UniformPtr;

  template<class ...TArgs>
  inline UniformPtr makeUniform(TArgs ...args) {
    return std::make_shared<Uniform>(std::forward<TArgs>(args)...);
  }

} // px

template<typename T>
px::Uniform &px::Uniform::operator=(const T &value) {
  bgfx::setUniform(m_handle, &value);
  return *this;
}

#endif //PX_ENGINE_UNIFORM_HPP