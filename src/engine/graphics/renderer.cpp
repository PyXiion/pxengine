// Copyright (c) 2023.

//
// Created by pyxiion on 04.11.23.
//

#include <bgfx/platform.h>
#include <easy/profiler.h>
#include "renderer.hpp"

static bgfx::ViewId viewsCount{};

namespace px {
  Renderer::Renderer(Window &window, bgfx::RendererType::Enum type)
    : m_viewId(viewsCount++) {
    EASY_BLOCK("Renderer::Renderer")
    // Call it before bgfx::init to signal to bgfx not to create a render thread.
    bgfx::renderFrame();

    // Init bgfx using the native window handle and window resolution
    bgfx::Init init;
    init.platformData.ndt = window.getDisplayHandle();
    init.platformData.nwh = window.getNativeHandle();
    init.type = type;

    m_reset = BGFX_RESET_VSYNC;

    int width, height;
    std::tie(width, height) = window.size();
    init.resolution.width  = width;
    init.resolution.height = height;
    init.resolution.reset  = m_reset;

    // Init bgfx
    if (!bgfx::init(init)) {
      throw std::runtime_error("Failed to init bgfx.");
    }

    bgfx::setViewClear(0
        , BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
        , 0x303030ff
        , 1.0f
        , 0
    );

    // Window resize callback
    listen(window.onFramebufferResize, [this](int width, int height) {
      bgfx::reset(width, height, m_reset);
      bgfx::setViewRect(0, 0, 0, width, height);

      m_framebufferSize = {width, height};
    });
  }

  Renderer::~Renderer() {
    bgfx::shutdown();
  }

  void Renderer::beginFrame() {
    EASY_BLOCK("Renderer::beginFrame")
    bgfx::touch(m_viewId);

    bgfx::setDebug(m_debugText ? BGFX_DEBUG_TEXT : (BGFX_DEBUG_STATS));
    if (m_debugText)
      debug();
  }

  void Renderer::renderFrame() {
    EASY_BLOCK("Renderer::renderFrame")
    bgfx::frame();
  }

  void Renderer::debug() {

  }

  bool Renderer::isDebugEnabled() const {
    return m_debugText;
  }

  void Renderer::setDebugEnabled(bool debugEnabled) {
    m_debugText = debugEnabled;
  }

  bgfx::ViewId Renderer::getViewId() const {
    return m_viewId;
  }

  px::Vector2i Renderer::getFramebufferSize() const {
    return m_framebufferSize;
  }
} // px