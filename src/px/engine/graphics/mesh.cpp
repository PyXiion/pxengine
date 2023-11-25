// Copyright (c) 2023.

//
// Created by pyxiion on 22.11.23.
//

#include "mesh.hpp"
#include <mutex>
#include <utility>

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

  Mesh::Mesh(std::vector<Vertex> vertices, std::vector<uint> indices, std::vector<TexturePtr> textures)
    : m_vertices(std::move(vertices))
    , m_indices(std::move(indices))
    , m_textures(std::move(textures)) {
    std::call_once(layoutInitOnce, initLayout);

    m_vb = createVertexBuffer();
    m_ib = createIndexBuffer();
  }

  void Mesh::apply(uint8_t stream) const {
    bgfx::setVertexBuffer(stream, m_vb);
    bgfx::setIndexBuffer(m_ib);
  }

  BgfxUniqueVertexBufferHandle Mesh::createVertexBuffer() {
    bgfx::VertexBufferHandle handle{};
    const std::size_t size = m_vertices.size() * sizeof(m_vertices[0]);
    handle = bgfx::createVertexBuffer(bgfx::makeRef(m_vertices.data(), size), layout); // TODO use layout from shader

    printf("Allocated a vertex buffer %zu bytes\n", size);
    return handle;
  }

  BgfxUniqueIndexBufferHandle Mesh::createIndexBuffer() {
    bgfx::IndexBufferHandle handle{};
    const std::size_t size = m_indices.size() * sizeof(m_indices[0]);
    handle = bgfx::createIndexBuffer(bgfx::makeRef(m_indices.data(), size));

    printf("Allocated an index buffer %zu bytes\n", size);
    return handle;
  }

} // px