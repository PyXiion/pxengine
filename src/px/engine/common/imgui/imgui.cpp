// Copyright (c) 2023.

//
// Created by pyxiion on 06.11.23.
//

#include "imgui.hpp"

#include <imgui/imgui_internal.h>
#include "../../engine.hpp"
#include "../../utils/bgfx_utils.hpp"
#include <bx/timer.h>
#include <bx/math.h>
#include <easylogging++.h>

#define IMGUI_FLAGS_ALPHA_BLEND UINT8_C(0x01)
#define IMGUI_MBUT_LEFT   0x01
#define IMGUI_MBUT_RIGHT  0x02
#define IMGUI_MBUT_MIDDLE 0x04

namespace px {

  ImGuiCtx::ImGuiCtx(px::Engine &engine, px::Window &window)
    : m_engine(engine) {
    std::tie(m_windowWidth, m_windowHeight) = window.size();

    listen(window.onKey, [this](auto key, bool down, auto mods){
      m_keys.emplace(key, down);
      m_mods = mods;
    });
    listen(window.onMouseMoved, [this](float x, float y) {
      m_mouseX = x;
      m_mouseY = y;
    });
    listen(window.onFramebufferResize, [this](float x, float y) {
      m_windowWidth =  int32_t(x);
      m_windowHeight = int32_t(y);
    });
    listen(window.onMouseClicked, [this](float x, float y, MouseButton btn, bool down) {
      m_mouseButtons[static_cast<std::size_t>(btn)] = down;
    });
    listen(window.onScroll, [this](float scrollX, float scrollY){
      m_scrollX = scrollX;
      m_scrollY = scrollY;
    });
    listen(window.onInput, [this](auto c){
      m_input.push(c);
    });
    CLOG(INFO, "PXEngine") << "Initialised the ImGui context";
  }

  void ImGuiCtx::Render(ImDrawData* _drawData) {
    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
    int fb_width = (int)(_drawData->DisplaySize.x * _drawData->FramebufferScale.x);
    int fb_height = (int)(_drawData->DisplaySize.y * _drawData->FramebufferScale.y);
    if (fb_width <= 0 || fb_height <= 0)
      return;

    bgfx::setViewName(m_viewId, "ImGui");
    bgfx::setViewMode(m_viewId, bgfx::ViewMode::Sequential);

    const bgfx::Caps* caps = bgfx::getCaps();
    {
      float ortho[16];
      float x = _drawData->DisplayPos.x;
      float y = _drawData->DisplayPos.y;
      float width = _drawData->DisplaySize.x;
      float height = _drawData->DisplaySize.y;

      bx::mtxOrtho(ortho, x, x + width, y + height, y, 0.0f, 1000.0f, 0.0f, caps->homogeneousDepth);
      bgfx::setViewTransform(m_viewId, nullptr, ortho);
      bgfx::setViewRect(m_viewId, 0, 0, uint16_t(width), uint16_t(height) );
    }

    const ImVec2 clipPos   = _drawData->DisplayPos;       // (0,0) unless using multi-viewports
    const ImVec2 clipScale = _drawData->FramebufferScale; // (1,1) unless using retina display which are often (2,2)


    // Render command lists
    for (int32_t ii = 0, num = _drawData->CmdListsCount; ii < num; ++ii)
    {
      bgfx::TransientVertexBuffer tvb{};
      bgfx::TransientIndexBuffer tib{};

      const ImDrawList* drawList = _drawData->CmdLists[ii];
      auto numVertices = (uint32_t)drawList->VtxBuffer.size();
      auto numIndices  = (uint32_t)drawList->IdxBuffer.size();

      if (!utils::bgfx::checkAvailTransientBuffers(numVertices, m_layout, numIndices) )
      {
        // not enough space in transient buffer just quit drawing the rest...
        printf("CALLED !!!!!!!!!1\n");
        break;
      }

      bgfx::allocTransientVertexBuffer(&tvb, numVertices, m_layout);
      bgfx::allocTransientIndexBuffer(&tib, numIndices, sizeof(ImDrawIdx) == 4);

      auto* verts = (ImDrawVert*)tvb.data;
      bx::memCopy(verts, drawList->VtxBuffer.begin(), numVertices * sizeof(ImDrawVert) );

      auto* indices = (ImDrawIdx*)tib.data;
      bx::memCopy(indices, drawList->IdxBuffer.begin(), numIndices * sizeof(ImDrawIdx) );

      bgfx::Encoder* encoder = bgfx::begin();

      for (const ImDrawCmd* cmd = drawList->CmdBuffer.begin(), *cmdEnd = drawList->CmdBuffer.end(); cmd != cmdEnd; ++cmd)
      {
        if (cmd->UserCallback)
        {
          cmd->UserCallback(drawList, cmd);
        }
        else if (0 != cmd->ElemCount)
        {
          uint64_t state = 0
                           | BGFX_STATE_WRITE_RGB
                           | BGFX_STATE_WRITE_A
                           | BGFX_STATE_MSAA
          ;

          bgfx::TextureHandle th = m_texture;
          bgfx::ProgramHandle program = m_shader->get();

          if (nullptr != cmd->TextureId)
          {
            union { ImTextureID ptr; struct { bgfx::TextureHandle handle; uint8_t flags; uint8_t mip; } s; } texture = { cmd->TextureId };
            state |= 0 != (IMGUI_FLAGS_ALPHA_BLEND & texture.s.flags)
                     ? BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA)
                     : BGFX_STATE_NONE
                ;
            th = texture.s.handle;
            if (0 != texture.s.mip)
            {
              const float lodEnabled[4] = { float(texture.s.mip), 1.0f, 0.0f, 0.0f };
              bgfx::setUniform(u_imageLodEnabled, lodEnabled);
              program = m_imageShader->get();
            }
          }
          else
          {
            state |= BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);
          }

          // Project scissor/clipping rectangles into framebuffer space
          ImVec4 clipRect;
          clipRect.x = (cmd->ClipRect.x - clipPos.x) * clipScale.x;
          clipRect.y = (cmd->ClipRect.y - clipPos.y) * clipScale.y;
          clipRect.z = (cmd->ClipRect.z - clipPos.x) * clipScale.x;
          clipRect.w = (cmd->ClipRect.w - clipPos.y) * clipScale.y;

          if (clipRect.x < float(fb_width)
              &&  clipRect.y <  float(fb_height)
              &&  clipRect.z >= 0.0f
              &&  clipRect.w >= 0.0f)
          {
            const uint16_t xx = uint16_t(bx::max(clipRect.x, 0.0f) );
            const uint16_t yy = uint16_t(bx::max(clipRect.y, 0.0f) );
            encoder->setScissor(xx, yy
                , uint16_t(bx::min(clipRect.z, 65535.0f)-xx)
                , uint16_t(bx::min(clipRect.w, 65535.0f)-yy)
            );

            encoder->setState(state);
            encoder->setTexture(0, s_tex, th);
            encoder->setVertexBuffer(0, &tvb, cmd->VtxOffset, numVertices);
            encoder->setIndexBuffer(&tib, cmd->IdxOffset, cmd->ElemCount);
            encoder->submit(m_viewId, program);
          }
        }
      }

      bgfx::end(encoder);
    }
  }

  void ImGuiCtx::Create() {
    CLOG(INFO, "PXEngine") << "Creating the ImGui context";
    IMGUI_CHECKVERSION();

    m_viewId = 255;
    m_last = bx::getHPCounter();

    m_imgui = ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();

    io.DisplaySize = ImVec2(1280.0f, 720.0f);
    io.DeltaTime = 1.0f / 60.0f;
    io.IniFilename = nullptr;

    setupStyle(true);

    io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

    #pragma region Keymap
    m_keyMap[KeyCode_Escape]       = ImGuiKey_Escape;
    m_keyMap[KeyCode_Enter]        = ImGuiKey_Enter;
    m_keyMap[KeyCode_Tab]          = ImGuiKey_Tab;
    m_keyMap[KeyCode_Space]        = ImGuiKey_Space;
    m_keyMap[KeyCode_Backspace]    = ImGuiKey_Backspace;
    m_keyMap[KeyCode_Up]           = ImGuiKey_UpArrow;
    m_keyMap[KeyCode_Down]         = ImGuiKey_DownArrow;
    m_keyMap[KeyCode_Left]         = ImGuiKey_LeftArrow;
    m_keyMap[KeyCode_Right]        = ImGuiKey_RightArrow;
    m_keyMap[KeyCode_Insert]       = ImGuiKey_Insert;
    m_keyMap[KeyCode_Delete]       = ImGuiKey_Delete;
    m_keyMap[KeyCode_Home]         = ImGuiKey_Home;
    m_keyMap[KeyCode_End]          = ImGuiKey_End;
    m_keyMap[KeyCode_PageUp]       = ImGuiKey_PageUp;
    m_keyMap[KeyCode_PageDown]     = ImGuiKey_PageDown;
    m_keyMap[KeyCode_PrintScreen]  = ImGuiKey_PrintScreen;
    m_keyMap[KeyCode_Equal]        = ImGuiKey_Equal;
    m_keyMap[KeyCode_Minus]        = ImGuiKey_Minus;
    m_keyMap[KeyCode_LeftBracket]  = ImGuiKey_LeftBracket;
    m_keyMap[KeyCode_RightBracket] = ImGuiKey_RightBracket;
    m_keyMap[KeyCode_Semicolon]    = ImGuiKey_Semicolon;
    m_keyMap[KeyCode_Apostrophe]   = ImGuiKey_Apostrophe;
    m_keyMap[KeyCode_Comma]        = ImGuiKey_Comma;
    m_keyMap[KeyCode_Period]       = ImGuiKey_Period;
    m_keyMap[KeyCode_Slash]        = ImGuiKey_Slash;
    m_keyMap[KeyCode_Backslash]    = ImGuiKey_Backslash;
//      m_keyMap[KeyCode_]        = ImGuiKey_GraveAccent;
    m_keyMap[KeyCode_F1]           = ImGuiKey_F1;
    m_keyMap[KeyCode_F2]           = ImGuiKey_F2;
    m_keyMap[KeyCode_F3]           = ImGuiKey_F3;
    m_keyMap[KeyCode_F4]           = ImGuiKey_F4;
    m_keyMap[KeyCode_F5]           = ImGuiKey_F5;
    m_keyMap[KeyCode_F6]           = ImGuiKey_F6;
    m_keyMap[KeyCode_F7]           = ImGuiKey_F7;
    m_keyMap[KeyCode_F8]           = ImGuiKey_F8;
    m_keyMap[KeyCode_F9]           = ImGuiKey_F9;
    m_keyMap[KeyCode_F10]          = ImGuiKey_F10;
    m_keyMap[KeyCode_F11]          = ImGuiKey_F11;
    m_keyMap[KeyCode_F12]          = ImGuiKey_F12;
//      m_keyMap[KeyCode_NumPad0]      = ImGuiKey_Keypad0;
//      m_keyMap[KeyCode_NumPad1]      = ImGuiKey_Keypad1;
//      m_keyMap[KeyCode_NumPad2]      = ImGuiKey_Keypad2;
//      m_keyMap[KeyCode_NumPad3]      = ImGuiKey_Keypad3;
//      m_keyMap[KeyCode_NumPad4]      = ImGuiKey_Keypad4;
//      m_keyMap[KeyCode_NumPad5]      = ImGuiKey_Keypad5;
//      m_keyMap[KeyCode_NumPad6]      = ImGuiKey_Keypad6;
//      m_keyMap[KeyCode_NumPad7]      = ImGuiKey_Keypad7;
//      m_keyMap[KeyCode_NumPad8]      = ImGuiKey_Keypad8;
//      m_keyMap[KeyCode_NumPad9]      = ImGuiKey_Keypad9;
    m_keyMap[KeyCode_0]         = ImGuiKey_0;
    m_keyMap[KeyCode_1]         = ImGuiKey_1;
    m_keyMap[KeyCode_2]         = ImGuiKey_2;
    m_keyMap[KeyCode_3]         = ImGuiKey_3;
    m_keyMap[KeyCode_4]         = ImGuiKey_4;
    m_keyMap[KeyCode_5]         = ImGuiKey_5;
    m_keyMap[KeyCode_6]         = ImGuiKey_6;
    m_keyMap[KeyCode_7]         = ImGuiKey_7;
    m_keyMap[KeyCode_8]         = ImGuiKey_8;
    m_keyMap[KeyCode_9]         = ImGuiKey_9;
    m_keyMap[KeyCode_A]         = ImGuiKey_A;
    m_keyMap[KeyCode_B]         = ImGuiKey_B;
    m_keyMap[KeyCode_C]         = ImGuiKey_C;
    m_keyMap[KeyCode_D]         = ImGuiKey_D;
    m_keyMap[KeyCode_E]         = ImGuiKey_E;
    m_keyMap[KeyCode_F]         = ImGuiKey_F;
    m_keyMap[KeyCode_G]         = ImGuiKey_G;
    m_keyMap[KeyCode_H]         = ImGuiKey_H;
    m_keyMap[KeyCode_I]         = ImGuiKey_I;
    m_keyMap[KeyCode_J]         = ImGuiKey_J;
    m_keyMap[KeyCode_K]         = ImGuiKey_K;
    m_keyMap[KeyCode_L]         = ImGuiKey_L;
    m_keyMap[KeyCode_M]         = ImGuiKey_M;
    m_keyMap[KeyCode_N]         = ImGuiKey_N;
    m_keyMap[KeyCode_O]         = ImGuiKey_O;
    m_keyMap[KeyCode_P]         = ImGuiKey_P;
    m_keyMap[KeyCode_Q]         = ImGuiKey_Q;
    m_keyMap[KeyCode_R]         = ImGuiKey_R;
    m_keyMap[KeyCode_S]         = ImGuiKey_S;
    m_keyMap[KeyCode_T]         = ImGuiKey_T;
    m_keyMap[KeyCode_U]         = ImGuiKey_U;
    m_keyMap[KeyCode_V]         = ImGuiKey_V;
    m_keyMap[KeyCode_W]         = ImGuiKey_W;
    m_keyMap[KeyCode_X]         = ImGuiKey_X;
    m_keyMap[KeyCode_Y]         = ImGuiKey_Y;
    m_keyMap[KeyCode_Z]         = ImGuiKey_Z;
#pragma endregion // Keymap

    io.ConfigFlags |= 0
        | ImGuiConfigFlags_NavEnableKeyboard
        ;

    auto &rm = px::Engine::getInstance().getResourceManager();
    m_shader = rm.get<Shader>("core.shaders.imgui");

    u_imageLodEnabled = bgfx::createUniform("u_imageLodEnabled", bgfx::UniformType::Vec4);
    m_imageShader = rm.get<Shader>("core.shaders.imgui_image");


    m_layout
      .begin()
      .add(bgfx::Attrib::Position,  2, bgfx::AttribType::Float)
      .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
      .add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Uint8, true)
      .end();

    s_tex = bgfx::createUniform("s_tex", bgfx::UniformType::Sampler);

    {
      ImFontConfig config;
      config.FontDataOwnedByAtlas = false;
      config.MergeMode = false;

      const ImWchar *ranges = io.Fonts->GetGlyphRangesCyrillic();

      auto &resources = m_engine.getResourceManager();

      auto regular = resources.get<std::vector<char>>("core.font.roboto.regular");
      auto italic  = resources.get<std::vector<char>>("core.font.roboto.italic");
      auto bold    = resources.get<std::vector<char>>("core.font.roboto.bold");

      io.Fonts->AddFontFromMemoryTTF(regular->data(), regular->size(), 16.0f, &config, ranges);
      io.Fonts->AddFontFromMemoryTTF(italic->data(),  italic->size(),  16.0f, &config, ranges);
      io.Fonts->AddFontFromMemoryTTF(bold->data(),    bold->size(),    16.0f, &config, ranges);
    }
    CLOG(INFO, "PXEngine") << "Loaded ImGui resources";

    uint8_t *data;
    int32_t width;
    int32_t height;
    io.Fonts->GetTexDataAsRGBA32(&data, &width, &height);

    m_texture = bgfx::createTexture2D(
          (uint16_t)width
        , (uint16_t)height
        , false
        , 1
        , bgfx::TextureFormat::BGRA8
        , 0
        , bgfx::copy(data, width*height*4)
        );

    CLOG(INFO, "PXEngine") << "The ImGui context is ready";
//      ImGui::InitDockContext();
  }

  void ImGuiCtx::Destroy() {
    ImGui::DestroyContext(m_imgui);
  }

  void ImGuiCtx::EndFrame() {
    ImGui::Render();
    Render(ImGui::GetDrawData());
  }

  void ImGuiCtx::setupStyle(bool dark) {
    ImGuiStyle &style = ImGui::GetStyle();
    if (dark) {
      ImGui::StyleColorsDark(&style);
    } else {
      ImGui::StyleColorsLight(&style);
    }

    style.FrameRounding = 4.0f;
    style.WindowBorderSize = 0.0f;

    CLOG(INFO, "PXEngine") << "Set up the " << (dark ? "dark" : "light") << " theme";
  }

  void ImGuiCtx::BeginFrame(bgfx::ViewId viewId) {
    m_viewId = viewId;

    ImGuiIO& io = ImGui::GetIO();

    io.DisplaySize = ImVec2( (float)m_windowWidth, (float)m_windowHeight);

    const int64_t now = bx::getHPCounter();
    const int64_t frameTime = now - m_last;
    m_last = now;
    const auto freq = double(bx::getHPFrequency() );
    io.DeltaTime = float(double(frameTime)/freq);

    io.AddMousePosEvent( m_mouseX, m_mouseY);
    io.AddMouseButtonEvent(ImGuiMouseButton_Left,   getMouseButtonState(MouseButton::Left) );
    io.AddMouseButtonEvent(ImGuiMouseButton_Right,  getMouseButtonState(MouseButton::Right) );
    io.AddMouseButtonEvent(ImGuiMouseButton_Middle, getMouseButtonState(MouseButton::Middle) );

    io.AddMouseWheelEvent(m_scrollX, m_scrollY);
    m_scrollX = 0; m_scrollY = 0;

    while (not m_input.empty()) {
      auto c = m_input.front();
      io.AddInputCharacter(c);
      m_input.pop();
    }

    io.AddKeyEvent(ImGuiKey_ModShift, 0 != (m_mods & KeyModifiers::Shift)   );
    io.AddKeyEvent(ImGuiKey_ModCtrl,  0 != (m_mods & KeyModifiers::Control) );
    io.AddKeyEvent(ImGuiKey_ModAlt,   0 != (m_mods & KeyModifiers::Alt)     );
    io.AddKeyEvent(ImGuiKey_ModSuper, 0 != (m_mods & KeyModifiers::Super)   );

    while (not m_keys.empty()) {
      auto [key, down] = m_keys.front();
      io.AddKeyEvent(m_keyMap[key], down);
      m_keys.pop();
    }

    ImGui::NewFrame();
  }

  bool ImGuiCtx::getMouseButtonState(MouseButton btn) {
    return m_mouseButtons[static_cast<std::size_t>(btn)];
  }
} // px

bool ImGui::InputVector3(const char *label, px::Vector3 &v, const char *format, ImGuiInputTextFlags flags) {
  [[maybe_unused]] bool result;
  float xyz[3] = {v.x, v.y, v.z};

  if (ImGui::InputFloat3(label, xyz, format, flags)) {
    v = {xyz[0], xyz[1], xyz[2]};
    return true;
  }
  return false;
}
