// Copyright (c) 2023.

//
// Created by pyxiion on 20.11.23.
//

#ifndef PX_ENGINE_SPRITE_RENDERER_HPP
#define PX_ENGINE_SPRITE_RENDERER_HPP
#include <mutex>
#include "px/engine/graphics/texture.hpp"
#include "px/engine/components/component.hpp"
#include "px/engine/graphics/mesh.hpp"
#include "px/engine/events/event_listener.hpp"
#include "px/engine/components/transform.hpp"

namespace px {
  class SpriteRenderer : public BaseComponent<"core.sprite_renderer">, EventListener {
  public:
    SpriteRenderer();

    explicit SpriteRenderer(const std::string &texture) {
      setTexture(texture);
    }

    void setTexture(const std::string &texture);

    void setTexture(Ref<Texture> texture);

    Ref<Texture> texture;

  private:
    Transform *m_transform{};
    RenderStates m_states;

    void draw();

    void subscribeEvents();

    void guiEditor() override;

    static Mesh mesh;
    static std::once_flag initMeshOnceFlag;

    static void generateMesh();
  };
} // px

#endif //PX_ENGINE_SPRITE_RENDERER_HPP
