// Copyright (c) 2023.

//
// Created by pyxiion on 04.11.23.
//

#ifndef ENGINE_RENDERER_HPP
#define ENGINE_RENDERER_HPP
#include <atomic>
#include <bgfx/bgfx.h>
#include "../system/window.hpp"
#include "../events/event_listener.hpp"
#include "../math/vector.hpp"

namespace px {

  class Renderer : protected EventListener {
  public:
    explicit Renderer(Window &window, bgfx::RendererType::Enum type = bgfx::RendererType::Count);
    ~Renderer();

    void beginFrame();
    void renderFrame();

    [[nodiscard]] bool isDebugEnabled() const;
    void setDebugEnabled(bool debugEnabled);

    [[nodiscard]] bgfx::ViewId getViewId() const;

    [[nodiscard]] px::Vector2i getFramebufferSize() const;

  private:
    std::uint32_t m_reset;
    bgfx::ViewId  m_viewId;
    px::Vector2i  m_framebufferSize{};

    std::atomic<bool> m_debugStats;

    void debug();
  };

} // px

#endif //ENGINE_RENDERER_HPP
