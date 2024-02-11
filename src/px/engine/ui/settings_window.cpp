// Copyright (c) 2023.

//
// Created by pyxiion on 22.11.23.
//

#include "settings_window.hpp"
#include "../engine.hpp"
#include "px/engine/resources/localization.hpp"

namespace px {
  SettingsWindow::SettingsWindow(Engine &engine)
    : m_engine(engine)
    , m_opened(false)
    , m_tab(Tab::Graphics) {
    engine.onInit.append([this](Engine &engine) {
      EASY_BLOCK("px::SettingsWindow::SettingsWindow -> px::Engine::onInit")
      m_localization = engine.getRegistries().LOCALIZATIONS.get("core");
      m_settings = &engine.getSettings();
    });
    listen(engine.onGuiDraw, [this] { onGuiDraw(); });
  }

  void SettingsWindow::setOpened(bool opened) {
    m_opened = opened;
  }

  bool SettingsWindow::isOpened() const {
    return m_opened;
  }

  void SettingsWindow::onGuiDraw() {
    EASY_BLOCK("SettingsWindow::onGuiDraw")
    if (not m_opened || not m_localization || not m_settings) return;
    std::string title = m_localization->get("ui.settings.title") + "##settings";
    if (ImGui::Begin(title.c_str(), &m_opened)) {

      if (ImGui::BeginTabBar("settings_tabs")) {
        bool changed = false;
        #pragma region Graphics
          GraphicsSettings *gs = &m_settings->graphicsSettings;
          if (ImGui::BeginTabItem(m_localization->getc("ui.settings.graphics.title"))) {
            changed |= ImGui::InputInt(m_localization->getc("ui.settings.graphics.max-fps"), &gs->maxFps, 1, 5);

            ImGui::EndTabItem();
          }
        #pragma endregion

        #pragma region Other
          if (ImGui::BeginTabItem(m_localization->getc("ui.settings.other.title"))) {
            ImGui::EndTabItem();
          }
          ImGui::EndTabBar();
        #pragma endregion

        // Reload settings
        if (changed)
          m_engine.reloadSettings();
      }

    }
    ImGui::End();
  }
} // px