// Copyright (c) 2023.

//
// Created by pyxiion on 22.11.23.
//

#include "mesh.hpp"
#include <mutex>
#include <utility>
#include <easylogging++.h>
#include <fmt/format.h>

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
  }

  Mesh::Mesh(std::vector<Vertex> vertices, std::vector<IndexType> indices, std::vector<Ref<Texture>> textures)
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

  void Mesh::create(std::vector<Vertex> vertices, std::vector<IndexType> indices, std::vector<Ref<Texture>> textures) {
    std::call_once(layoutInitOnce, initLayout);
    m_vertices = std::move(vertices);
    m_indices = std::move(indices);
    m_textures = std::move(textures);

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


    for (auto i = 0; i < m_textures.size(); i++) {
      bgfx::setTexture(i, s_texture, m_textures[i]->getHandle());
    }

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

  void Mesh::setTexture(std::size_t i, Ref<Texture> texture) {
    if (m_textures.size() < i) {
      m_textures[i] = std::move(texture);
    } else {
      CLOG(ERROR, "PXEngine") << "Mesh::setTexture out of range.";
      CLOG(ERROR, "PXEngine") << "\t";
      throw std::out_of_range(fmt::format("setTexture({0}, {1}) ot of range. m_textures.size() < i === {2} < {0}", i, fmt::ptr(texture.get()), m_textures.size()));
    }
  }

  void Mesh::setTextures(std::vector<Ref<Texture>> textures) {
    m_textures = std::move(textures);
  }

  std::vector<Ref<Texture>> Mesh::getTextures() const {
    return m_textures;
  }

} // px