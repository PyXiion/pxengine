//
// Created by pyxiion on 21.10.23.
//

#include <mutex>
#include <easy/profiler.h>
#include "cube.hpp"
#include "../engine.hpp"

namespace px {
  bgfx::VertexLayout PosColorVertex::ms_layout;

  static PosColorVertex s_cubeVertices[] = {
      { 1.0f,  1.0f, -1.0f, 0xff000000 },
      { 1.0f, -1.0f, -1.0f, 0xff0000ff },
      { 1.0f,  1.0f,  1.0f, 0xff00ff00 },
      { 1.0f, -1.0f,  1.0f, 0xff00ffff },
      {-1.0f,  1.0f, -1.0f, 0xffff0000 },
      {-1.0f, -1.0f, -1.0f, 0xffff00ff },
      {-1.0f,  1.0f,  1.0f, 0xffffff00 },
      {-1.0f, -1.0f,  1.0f, 0xffffffff },
  };

  static const uint16_t s_cubeTriList[] = {
      4, 2, 0, // 0
      2, 7, 3,
      6, 5, 7, // 2
      1, 7, 5,
      0, 3, 1, // 4
      4, 1, 5,
      4, 6, 2, // 6
      2, 6, 7,
      6, 4, 5, // 8
      1, 3, 7,
      0, 2, 3, // 10
      4, 0, 1,
  };

  std::once_flag initVertices;
  Cube::Cube() {
    EASY_BLOCK("px::Cube::Cube")
    std::call_once(initVertices, &PosColorVertex::init);
    createBuffers();

    m_program = px::Engine::getInstance().getResourceManager().loadProgram("core.shaders.vs_cubes", "core.shaders.fs_cubes");
  }
  Cube::~Cube() {
  }

  void Cube::createBuffers() {
    EASY_BLOCK("Cube::createBuffers")
    // Create static vertex buffer.
    m_vbh = bgfx::createVertexBuffer(
        // Static data can be passed with bgfx::makeRef
        bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices) )
        , PosColorVertex::ms_layout
    );

    // Create static index buffer for triangle list rendering.
    m_ibh = bgfx::createIndexBuffer(
        // Static data can be passed with bgfx::makeRef
        bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList) )
    );

    auto &rm = Engine::getInstance().getResourceManager();
    rm["core.cube.vb"] = m_vbh;
    rm["core.cube.ib"] = m_ibh;
  }

  void Cube::draw(int i) {
    EASY_BLOCK("Cube::draw")
    constexpr uint64_t state = 0
                     | BGFX_STATE_WRITE_R
                     | BGFX_STATE_WRITE_G
                     | BGFX_STATE_WRITE_B
                     | BGFX_STATE_WRITE_A
                     | BGFX_STATE_WRITE_Z
                     | BGFX_STATE_DEPTH_TEST_LESS
                     | BGFX_STATE_CULL_CW
                     | BGFX_STATE_MSAA
    ;

    bgfx::setVertexBuffer(0, m_vbh);
    bgfx::setIndexBuffer(m_ibh);
    bgfx::setState(state);
    bgfx::submit(0, m_program);
  }
} // px