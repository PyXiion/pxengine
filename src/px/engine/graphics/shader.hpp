// Copyright (c) 2023.

//
// Created by pyxiion on 22.11.23.
//

#ifndef PX_ENGINE_SHADER_HPP
#define PX_ENGINE_SHADER_HPP
#include <memory>
#include <bgfx/bgfx.h>
#include "px/engine/resources/bgfx_handle.hpp"
#include "px/memory/ref_counting.hpp"

namespace px {
  class Shader final : public RefCounting {
  public:
    Shader(bgfx::ShaderHandle vs, bgfx::ShaderHandle fs);
    ~Shader() override = default;

    [[nodiscard]] bgfx::ProgramHandle get() const;

    static std::string getCurrentRendererTypeSuffix();

  private:
    BgfxUniqueProgramHandle m_program;
  };
} // px

#endif //PX_ENGINE_SHADER_HPP
