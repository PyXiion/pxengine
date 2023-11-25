// Copyright (c) 2023.

//
// Created by pyxiion on 24.11.23.
//

#ifndef PX_ENGINE_MODEL_RENDERER_HPP
#define PX_ENGINE_MODEL_RENDERER_HPP

#include "px/engine/components/component.hpp"
#include "px/engine/graphics/model.hpp"
#include "px/engine/events/event_listener.hpp"
#include "px/engine/components/transform.hpp"

namespace px {

  class ModelRenderer : public Component, protected EventListener {
    PX_COMPONENT_DECLARATION

  public:
    ModelRenderer();

    void setModel(ModelPtr model);
    void setRenderStates(RenderStates renderStates);

  private:
    ModelPtr m_model;

    Transform *m_transform{};
    RenderStates m_states;

    void subscribeEvents();
    void draw();
  };

} // px

#endif //PX_ENGINE_MODEL_RENDERER_HPP
