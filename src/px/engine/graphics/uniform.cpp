// Copyright (c) 2023.

//
// Created by pyxiion on 07.12.23.
//

#include "uniform.hpp"

namespace px {
  Uniform::Uniform(const std::string &name, bgfx::UniformType::Enum type) {
    create(name, type);
  }

  Uniform::Uniform() {
    m_handle = {bgfx::kInvalidHandle};
  }

  void Uniform::create(const std::string &name, bgfx::UniformType::Enum type) {
    m_handle = bgfx::createUniform(name.c_str(), type);
  }

  Uniform::operator bgfx::UniformHandle() const {
    return m_handle;
  }

  Uniform::operator bool() const {
    return static_cast<bool>(m_handle);
  }
} // px