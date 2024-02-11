//
// Created by pyxiion on 22.11.23.
//

#include "shader.hpp"
#include <filesystem>
#include "px/engine/utils/bgfx_utils.hpp"
#include "model.hpp"
#include "px/engine/settings.hpp"


namespace fs = std::filesystem;

namespace px {
  Shader::Shader(bgfx::ShaderHandle vs, bgfx::ShaderHandle fs) {
    m_program = bgfx::createProgram(vs, fs, true);
  }

  bgfx::ProgramHandle Shader::get() const {
    return m_program;
  }

  std::string Shader::getCurrentRendererTypeSuffix() {
    switch (bgfx::getRendererType()) {
      case bgfx::RendererType::OpenGL:
        return "glsl";
      case bgfx::RendererType::Vulkan:
        return "spirv";
      default:
        CLOG(FATAL, "PXEngine") << "Unknown renderer type";
        throw std::runtime_error("Unknown renderer type");
    }
  }
} // px