// Copyright (c) 2023.

//
// Created by pyxiion on 07.12.23.
//

#include "uniform.hpp"

namespace px {
  Uniform::Uniform(const std::string &name, bgfx::UniformType::Enum type) {
    m_handle = bgfx::createUniform(name.c_str(), type);
  }
} // px