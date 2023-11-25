// Copyright (c) 2023.

//
// Created by pyxiion on 24.11.23.
//

#include "model_renderer.hpp"
#include <utility>
#include "../../engine.hpp"

namespace px {
  PX_COMPONENT_DEFINITION(Component, ModelRenderer)

  ModelRenderer::ModelRenderer() {
    subscribeEvents();

    m_transform = getGameObject()->getComponent<px::Transform>();
  }

  void ModelRenderer::subscribeEvents() {
    auto &engine = getGameObject()->getWorld().getEngine();
    listen(engine.onDraw, [this] {draw();});
  }

  void ModelRenderer::draw() {
    if (m_model and m_states.shaderPtr and m_transform) {
      m_model->draw(m_states);
    }
  }

  void ModelRenderer::setModel(ModelPtr model) {
    m_model = std::move(model);
  }

  void ModelRenderer::setRenderStates(RenderStates renderStates) {
    m_states = std::move(renderStates);
  }
} // px