// Copyright (c) 2023.

//
// Created by pyxiion on 27.11.23.
//

#ifndef PX_ENGINE_KEY_EVENT_HPP
#define PX_ENGINE_KEY_EVENT_HPP
#include "../event.hpp"
#include "../../system/keycodes.hpp"
#include "../event_fabric.hpp"

namespace px {
  // core.event.key
  struct KeyEvent final : public Event {
    PX_EVENT_DECLARATION(KeyEvent, "core.event.key")

  public:
    ~KeyEvent() override = default;

    KeyCode code{};
    bool released{};
  };

  // core.event.key.up
  struct KeyReleasedEvent final : public Event {
    PX_EVENT_DECLARATION(KeyReleasedEvent, "core.event.key.up")

  public:
    ~KeyReleasedEvent() override = default;

    KeyCode code{};
  };

  // core.event.key.down
  struct KeyPressedEvent final : public Event {
    ~KeyPressedEvent() override = default;
    PX_EVENT_DECLARATION(KeyPressedEvent, "core.event.key.down")

  public:

    KeyCode code{};
  };
} // px

#endif //PX_ENGINE_KEY_EVENT_HPP
