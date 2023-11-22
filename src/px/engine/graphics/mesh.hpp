// Copyright (c) 2023.

//
// Created by pyxiion on 22.11.23.
//

#ifndef PX_ENGINE_MESH_HPP
#define PX_ENGINE_MESH_HPP
#include <string>
#include <vector>
#include "vertex.hpp"
#include "texture.hpp"

namespace px {

  class Mesh {
  public:
    Mesh();
    Mesh(const std::string &filename);

    void loadFromFile(const std::string &filename);

  private:
    std::vector<Vertex> m_vertices;
    std::vector<TexturePtr> m_textures;

    static bgfx::VertexLayout layout;
    static void initLayout();
  };

} // px

#endif //PX_ENGINE_MESH_HPP
