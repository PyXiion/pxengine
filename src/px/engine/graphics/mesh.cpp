// Copyright (c) 2023.

//
// Created by pyxiion on 22.11.23.
//

#include "mesh.hpp"
#include <mutex>
#include <utility>
#include <easylogging++.h>

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

  Mesh::Mesh(std::vector<Vertex> vertices, std::vector<IndexType> indices, std::vector<TexturePtr> textures)
    : m_vertices(std::move(vertices))
    , m_indices(std::move(indices))
    , m_textures(std::move(textures)) {
    std::call_once(layoutInitOnce, initLayout);

    m_vb = createVertexBuffer();
    m_ib = createIndexBuffer();

    if (not s_texture) {
      s_texture.create("s_texColor", bgfx::UniformType::Sampler);
    }
  }

  void Mesh::submit(const RenderStates &renderStates) const {
    uint64_t state =
          BGFX_STATE_WRITE_RGB
        | BGFX_STATE_WRITE_A
        | BGFX_STATE_WRITE_Z
        | BGFX_STATE_DEPTH_TEST_LESS
        | BGFX_STATE_CULL_CW
        | BGFX_STATE_MSAA
    ;


    bgfx::setTexture(0, s_texture, m_textures[0]->getHandle());

    bgfx::setState(state);
    bgfx::setVertexBuffer(0, m_vb);
    bgfx::setIndexBuffer(m_ib);

    bgfx::submit(renderStates.viewId,
                 renderStates.shaderPtr->get());
  }

  BgfxUniqueVertexBufferHandle Mesh::createVertexBuffer() {
    bgfx::VertexBufferHandle handle{};
    const std::size_t size = m_vertices.size() * sizeof(m_vertices[0]);
    handle = bgfx::createVertexBuffer(bgfx::makeRef(m_vertices.data(), size), layout); // TODO use layout from shader

    CLOG(INFO, "PXEngine") << "Created a vertex buffer at " << handle.idx
                           << " (Size: " << size << " bytes"
                           << ", vertices: " << m_vertices.size() << ")";

    return handle;
  }

  BgfxUniqueIndexBufferHandle Mesh::createIndexBuffer() {
    bgfx::IndexBufferHandle handle{};
    const std::size_t size = m_indices.size() * sizeof(m_indices[0]);
    handle = bgfx::createIndexBuffer(bgfx::makeRef(m_indices.data(), size));

    CLOG(INFO, "PXEngine") << "Created an index buffer at " << handle.idx
                           << " (Size: " << size << " bytes"
                           << ", indices: " << m_indices.size() << ")";
    return handle;
  }

  std::vector<TexturePtr> Mesh::getTextures() const {
    return m_textures;
  }

} // px