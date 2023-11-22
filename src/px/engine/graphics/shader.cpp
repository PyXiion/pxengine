//
// Created by pyxiion on 22.11.23.
//

#include "shader.hpp"

namespace px {
  Shader::Shader(bgfx::ShaderHandle vs, bgfx::ShaderHandle fs) {
    m_program = bgfx::createProgram(vs, fs, false);
  }

  bgfx::ProgramHandle Shader::get() const {
    return m_program;
  }
} // px