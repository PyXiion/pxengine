// Copyright (c) 2023.

//
// Created by pyxiion on 22.11.23.
//

#include "mesh.hpp"
#include <mutex>

namespace px {
  static std::once_flag layoutInitOnce;
  bgfx::VertexLayout px::Mesh::layout;
  void Mesh::initLayout() {
    layout
      .begin()
        .add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Normal,    3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
      .end();
  }

  Mesh::Mesh() {
    std::call_once(layoutInitOnce, initLayout);
  }

  Mesh::Mesh(const std::string &filename)
    : Mesh() {
    loadFromFile(filename);
  }
} // px