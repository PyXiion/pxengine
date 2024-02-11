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
  class ModelRenderer : public BaseComponent<"core.model_renderer">, protected EventListener {
  public:
    ModelRenderer();

    explicit ModelRenderer(const std::string &model);

    void setModel(Ref<Model> model);

    void setModel(const std::string &model);

    void setRenderStates(RenderStates renderStates);

    void guiEditor() override;

  private:
    Ref<Model> m_model;

    Transform *m_transform{};
    RenderStates m_states;

    void subscribeEvents();

    void draw();
  };
} // px

#endif //PX_ENGINE_MODEL_RENDERER_HPP
