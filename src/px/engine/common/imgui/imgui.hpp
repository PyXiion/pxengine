// Copyright (c) 2023.

//
// Created by pyxiion on 06.11.23.
//

#ifndef ENGINE_IMGUI_HPP
#define ENGINE_IMGUI_HPP
#include <memory>
#include <queue>
#include <bgfx/bgfx.h>
#include <imgui/imgui.h>
#include <fmt/format.h>
#include "../../system/keycodes.hpp"
#include "../../resources/bgfx_handle.hpp"
#include "../../events/event_listener.hpp"
#include "../../graphics/shader.hpp"
#include "../../math/vector.hpp"

namespace px {
  class Engine;
  class Window;

  class ImGuiCtx : private EventListener {
  public:
    explicit ImGuiCtx(px::Engine &engine, px::Window &window);

    void Create();
    void Destroy();

    void BeginFrame(bgfx::ViewId viewId);
    void EndFrame();

  private:
    Engine &m_engine;

    bool m_wasDestroyed = false;

    px::KeyCode m_lastKey;
    int32_t m_windowWidth{};
    int32_t m_windowHeight{};

    float m_mouseX{};
    float m_mouseY{};

    float m_scrollX{};
    float m_scrollY{};

    bool m_clicked = false;
    bool m_mouseButtons[static_cast<unsigned long>(MouseButton::Count)];

    std::queue<unsigned int> m_input;
    std::queue<std::pair<px::KeyCode, bool>> m_keys; // pair<key, down>
    KeyModifiers::Enum m_mods;

    ImGuiContext *m_imgui{};
    bgfx::VertexLayout m_layout;

    ShaderPtr m_shader;
    ShaderPtr m_imageShader;

    BgfxUniqueTextureHandle m_texture;
    BgfxUniqueUniformHandle s_tex;
    BgfxUniqueUniformHandle u_imageLodEnabled;
//    ImFont* m_font[ImGui::Font::Count];
    int64_t m_last{};

    bgfx::ViewId m_viewId{};

    ImGuiKey m_keyMap[(int) KeyCode_Count];

    void Render(ImDrawData* drawData);
    void setupStyle(bool dark);
    bool getMouseButtonState(MouseButton btn);
  };
} // px

namespace ImGui {
#define PX_IMGUI_OVERLOAD_FORMATTED(funcName) \
  template <class... T> \
  inline static void funcName ## Fmt(const std::string &formatStr, T &&...args) { \
    ImGui::funcName("%s", fmt::format(fmt::runtime(formatStr), args...).c_str()); \
  }

  PX_IMGUI_OVERLOAD_FORMATTED(Text)
  PX_IMGUI_OVERLOAD_FORMATTED(SetTooltip)

#undef PX_IMGUI_OVERLOAD_FORMATTED

  bool InputVector3(const char *label, px::Vector3 &v, const char *format = "%.3f", ImGuiInputTextFlags flags = 0);
}

#endif //ENGINE_IMGUI_HPP
