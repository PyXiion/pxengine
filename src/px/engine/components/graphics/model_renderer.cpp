// Copyright (c) 2023.

//
// Created by pyxiion on 24.11.23.
//

#include "model_renderer.hpp"
#include <utility>
#include "px/engine/engine.hpp"

namespace px {
  ModelRenderer::ModelRenderer() {
    subscribeEvents();

    m_transform = getGameObject()->getComponent<px::Transform>();
  }

  ModelRenderer::ModelRenderer(const std::string &model) {
    setModel(model);
  }

  void ModelRenderer::subscribeEvents() {
    auto engine = getGameObject()->getEngine();
    listen(engine->onDraw, &ModelRenderer::draw);
  }

  void ModelRenderer::draw() {
    if (m_model and m_states.shaderPtr and m_transform) {
      m_model->draw(m_states);
    }
  }

  void ModelRenderer::setModel(ModelPtr model) {
    m_model = std::move(model);
  }

  void ModelRenderer::setModel(const std::string &model) {
    auto &resources = getGameObject()->getEngine()->getResourceManager();
    setModel(resources.loadModel(model));
  }

  void ModelRenderer::setRenderStates(RenderStates renderStates) {
    m_states = std::move(renderStates);
  }

  void ModelRenderer::guiEditor() {
    // draw texture
    for (auto &mesh : m_model->getMeshes()) {
      for (auto &texture : mesh.getTextures()) {
        ImGui::Image(texture->getHandle(), {0, 100});
      }
    }
  }
} // px