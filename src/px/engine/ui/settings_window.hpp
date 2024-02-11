// Copyright (c) 2023.

//
// Created by pyxiion on 22.11.23.
//

#ifndef PX_ENGINE_SETTINGS_WINDOW_HPP
#define PX_ENGINE_SETTINGS_WINDOW_HPP

#include "px/engine/events/event_listener.hpp"
#include "px/engine/settings.hpp"
#include "px/memory/ref.hpp"

namespace px {
  class Engine;
  class Localization;

  class SettingsWindow : protected EventListener {
  private:
    enum class Tab {
      Graphics
    };

  public:
    explicit SettingsWindow(Engine &engine);

    void setOpened(bool opened);
    bool isOpened() const;

  private:
    Engine &m_engine;
    Settings *m_settings;
    Ref<Localization> m_localization;

    Tab m_tab;
    bool m_opened;

    void onGuiDraw();
  };

} // px

#endif //PX_ENGINE_SETTINGS_WINDOW_HPP
