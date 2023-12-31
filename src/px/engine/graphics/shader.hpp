// Copyright (c) 2023.

//
// Created by pyxiion on 22.11.23.
//

#ifndef PX_ENGINE_SHADER_HPP
#define PX_ENGINE_SHADER_HPP
#include <memory>
#include <bgfx/bgfx.h>
#include "px/engine/resources/bgfx_handle.hpp"
#include "px/engine/resources/resource_traits.hpp"

namespace px {

  class Shader {
  public:
    Shader(bgfx::ShaderHandle vs, bgfx::ShaderHandle fs);

    [[nodiscard]] bgfx::ProgramHandle get() const;

  private:
    BgfxUniqueProgramHandle m_program;
  };

  typedef std::shared_ptr<Shader> ShaderPtr;

  template <class ...TArgs>
  inline static ShaderPtr makeShader(TArgs ...args) { return std::make_shared<Shader>(std::forward<TArgs>(args)...); }

  namespace resources {
    template<>
    struct Traits<BgfxUniqueShaderHandle> {
      static std::string getPath(const std::string &root, const std::string &resourceId);

      static Resource<BgfxUniqueShaderHandle> load(std::istream &is);
    };

    template<>
    struct Traits<Shader> {
      static std::vector<std::string> extensions;

      static Resource<Shader> load(ResourceManager &manager, std::istream &is);
    };
  } // resources
} // px

#endif //PX_ENGINE_SHADER_HPP
