// Copyright (c) 2023.

//
// Created by pyxiion on 20.11.23.
//

#include "sprite_renderer.hpp"
#include "px/engine/engine.hpp"
#include <utility>

namespace px {
  Mesh SpriteRenderer::mesh;
  std::once_flag SpriteRenderer::initMeshOnceFlag;
  void SpriteRenderer::generateMesh() {
    std::vector<Vertex> vertices = {
        {{0, 0, 0}, {0, 0, -1}, {0, 0}},
        {{0, 1, 0}, {0, 0, -1}, {0, 1}},
        {{1, 1, 0}, {0, 0, -1}, {1, 1}},
        {{1, 0, 0}, {0, 0, -1}, {1, 0}},
    };
    std::vector<IndexType> indices = {
        0, 1, 2,
        2, 3, 0
    };
    mesh.create(vertices, indices, {});
    CLOG(INFO, "PXEngine") << "Generated a sprite mesh";
  }

  SpriteRenderer::SpriteRenderer() {
    std::call_once(initMeshOnceFlag, &generateMesh);

    m_transform = getGameObject()->getComponent<px::Transform>();
  }

  void SpriteRenderer::draw() {
    if (texture and m_transform and m_states.shaderPtr) {
      mesh.submit(m_states);
    }
  }

  void SpriteRenderer::guiEditor() {

  }

  void SpriteRenderer::setTexture(const std::string &textureId) {
    auto &rm = getGameObject()->getEngine()->getResourceManager();
    setTexture(rm.get<Texture>(textureId));
  }

  void SpriteRenderer::setTexture(TexturePtr newTexture) {
    texture = std::move(newTexture);
  }

  void SpriteRenderer::subscribeEvents() {
    auto engine = getGameObject()->getEngine();
    listen(engine->onDraw, &SpriteRenderer::draw);
  }
} // px