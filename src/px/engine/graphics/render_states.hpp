// Copyright (c) 2023.

//
// Created by pyxiion on 25.11.23.
//

#ifndef PX_ENGINE_RENDER_STATES_HPP
#define PX_ENGINE_RENDER_STATES_HPP
#include <bgfx/bgfx.h>
#include "../math/matrix.hpp"
#include "shader.hpp"

namespace px {

  struct RenderStates {
    ShaderPtr shaderPtr;
    bgfx::ViewId viewId{};
  };

} // px

#endif //PX_ENGINE_RENDER_STATES_HPP
