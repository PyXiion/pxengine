//
// Created by pyxiion on 21.10.23.
//

#ifndef ENGINE_CUBE_HPP
#define ENGINE_CUBE_HPP
#include <bgfx/bgfx.h>

namespace px {

  struct PosColorVertex {
    float m_x;
    float m_y;
    float m_z;
    uint32_t m_abgr;

    inline static void init() {
      ms_layout
          .begin()
          .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
          .add(bgfx::Attrib::Color0,   4, bgfx::AttribType::Uint8, true)
          .end();
    };

    static bgfx::VertexLayout ms_layout;
  };

  class Cube {
  public:
    Cube();
    ~Cube();

    void draw(int i);

  private:
    void createBuffers();

    bgfx::VertexBufferHandle m_vbh{};
    bgfx::IndexBufferHandle m_ibh[5]{};
    bgfx::ProgramHandle m_program{};
  };

} // px

#endif //ENGINE_CUBE_HPP
