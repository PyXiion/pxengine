//
// Created by pyxiion on 21.10.23.
//

#include <mutex>
#include <easy/profiler.h>
#include "cube.hpp"
#include "../engine.hpp"

namespace px {
  bgfx::VertexLayout PosColorVertex::ms_layout;

  static PosColorVertex s_cubeVertices[] =
      {
          {-1.0f,  1.0f,  1.0f, 0xff000000 },
          { 1.0f,  1.0f,  1.0f, 0xff0000ff },
          {-1.0f, -1.0f,  1.0f, 0xff00ff00 },
          { 1.0f, -1.0f,  1.0f, 0xff00ffff },
          {-1.0f,  1.0f, -1.0f, 0xffff0000 },
          { 1.0f,  1.0f, -1.0f, 0xffff00ff },
          {-1.0f, -1.0f, -1.0f, 0xffffff00 },
          { 1.0f, -1.0f, -1.0f, 0xffffffff },
      };

  static const uint16_t s_cubeTriList[] =
      {
          0, 1, 2, // 0
          1, 3, 2,
          4, 6, 5, // 2
          5, 6, 7,
          0, 2, 4, // 4
          4, 2, 6,
          1, 5, 3, // 6
          5, 7, 3,
          0, 4, 1, // 8
          4, 5, 1,
          2, 3, 6, // 10
          6, 3, 7,
      };

  static const uint16_t s_cubeTriStrip[] =
      {
          0, 1, 2,
          3,
          7,
          1,
          5,
          0,
          4,
          2,
          6,
          7,
          4,
          5,
      };

  static const uint16_t s_cubeLineList[] =
      {
          0, 1,
          0, 2,
          0, 4,
          1, 3,
          1, 5,
          2, 3,
          2, 6,
          3, 7,
          4, 5,
          4, 6,
          5, 7,
          6, 7,
      };

  static const uint16_t s_cubeLineStrip[] =
      {
          0, 2, 3, 1, 5, 7, 6, 4,
          0, 2, 6, 4, 5, 7, 3, 1,
          0,
      };

  static const uint16_t s_cubePoints[] =
      {
          0, 1, 2, 3, 4, 5, 6, 7
      };

  static const char* s_ptNames[]
      {
          "Triangle List",
          "Triangle Strip",
          "Lines",
          "Line Strip",
          "Points",
      };

  static const uint64_t s_ptState[]
      {
          UINT64_C(0),
          BGFX_STATE_PT_TRISTRIP,
          BGFX_STATE_PT_LINES,
          BGFX_STATE_PT_LINESTRIP,
          BGFX_STATE_PT_POINTS,
      };

#define PX_COUNTOF(a) (sizeof(a) / sizeof(*a))
  static_assert(PX_COUNTOF(s_ptState) == PX_COUNTOF(s_ptNames));
#undef PX_COUNTOF

  std::once_flag initVertices;
  Cube::Cube() {
    EASY_BLOCK("px::Cube::Cube")
    std::call_once(initVertices, &PosColorVertex::init);
    createBuffers();

    m_program = px::Engine::getInstance().getResourceManager().loadProgram("vs_cubes", "fs_cubes");
  }
  Cube::~Cube() {
    bgfx::destroy(m_program);

    bgfx::destroy(m_vbh);
    for (auto ibh : m_ibh) {
      bgfx::destroy(ibh);
    }
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
    m_ibh[0] = bgfx::createIndexBuffer(
        // Static data can be passed with bgfx::makeRef
        bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList) )
    );

    // Create static index buffer for triangle strip rendering.
    m_ibh[1] = bgfx::createIndexBuffer(
        // Static data can be passed with bgfx::makeRef
        bgfx::makeRef(s_cubeTriStrip, sizeof(s_cubeTriStrip) )
    );

    // Create static index buffer for line list rendering.
    m_ibh[2] = bgfx::createIndexBuffer(
        // Static data can be passed with bgfx::makeRef
        bgfx::makeRef(s_cubeLineList, sizeof(s_cubeLineList) )
    );

    // Create static index buffer for line strip rendering.
    m_ibh[3] = bgfx::createIndexBuffer(
        // Static data can be passed with bgfx::makeRef
        bgfx::makeRef(s_cubeLineStrip, sizeof(s_cubeLineStrip) )
    );

    // Create static index buffer for point list rendering.
    m_ibh[4] = bgfx::createIndexBuffer(
        // Static data can be passed with bgfx::makeRef
        bgfx::makeRef(s_cubePoints, sizeof(s_cubePoints) )
    );

    bgfx::frame();
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
    bgfx::setIndexBuffer(m_ibh[i]);
    bgfx::setState(state);
    bgfx::submit(0, m_program);
  }
} // px