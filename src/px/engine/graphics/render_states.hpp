// Copyright (c) 2023.

//
// Created by pyxiion on 25.11.23.
//

#ifndef PX_ENGINE_RENDER_STATES_HPP
#define PX_ENGINE_RENDER_STATES_HPP
#include <bgfx/bgfx.h>
#include "../math/matrix.hpp"
#include "shader.hpp"
#include "px/memory/ref.hpp"

namespace px {
  struct RenderStates {
    static constexpr uint64_t defaultState =
        BGFX_STATE_WRITE_RGB
        | BGFX_STATE_WRITE_A
        | BGFX_STATE_WRITE_Z
        | BGFX_STATE_DEPTH_TEST_LESS
        | BGFX_STATE_CULL_CW
        | BGFX_STATE_MSAA;

    Ref<Shader> shaderPtr;
    bgfx::ViewId viewId{};

    uint64_t state = defaultState;
  };
} // px

#endif //PX_ENGINE_RENDER_STATES_HPP
