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
    Mesh(std::vector<Vertex> vertices, std::vector<uint> indices, std::vector<TexturePtr> textures);

    void apply(uint8_t stream) const;

  private:
    std::vector<Vertex> m_vertices;
    std::vector<uint> m_indices;
    std::vector<TexturePtr> m_textures;

    static bgfx::VertexLayout layout;
    static void initLayout();

    BgfxUniqueVertexBufferHandle m_vb;
    BgfxUniqueIndexBufferHandle m_ib;

    BgfxUniqueVertexBufferHandle createVertexBuffer();
    BgfxUniqueIndexBufferHandle  createIndexBuffer();
  };

} // px

#endif //PX_ENGINE_MESH_HPP
