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
  struct KeyEvent final : public BaseEvent<KeyEvent, "core.event.key"> {
    ~KeyEvent() override = default;

    KeyCode code{};
    bool released{};
  };

  struct KeyReleasedEvent final : public BaseEvent<KeyReleasedEvent, "core.event.key.up"> {
    ~KeyReleasedEvent() override = default;
    KeyCode code{};
  };

  struct KeyPressedEvent final : public BaseEvent<KeyPressedEvent, "core.event.key.down"> {
    ~KeyPressedEvent() override = default;
    KeyCode code{};
  };
} // px

#endif //PX_ENGINE_KEY_EVENT_HPP
