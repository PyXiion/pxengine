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
    inline KeyEvent(KeyCode code, bool pressed)
      : code(code), pressed(pressed) {}
    ~KeyEvent() override = default;

    KeyCode code{};
    bool pressed{};
  };

  struct KeyReleasedEvent final : public BaseEvent<KeyReleasedEvent, "core.event.key.up"> {
    inline explicit KeyReleasedEvent(KeyCode code)
    : code(code) {}
    ~KeyReleasedEvent() override = default;
    KeyCode code{};
  };

  struct KeyPressedEvent final : public BaseEvent<KeyPressedEvent, "core.event.key.down"> {
    inline explicit KeyPressedEvent(KeyCode code)
    : code(code) {}
    ~KeyPressedEvent() override = default;
    KeyCode code{};
  };

  typedef CustomEventPtr<KeyEvent> KeyEventPtr;
  typedef CustomEventPtr<KeyReleasedEvent> KeyReleasedEventPtr;
  typedef CustomEventPtr<KeyPressedEvent> KeyPressedEventPtr;
} // px

#endif //PX_ENGINE_KEY_EVENT_HPP
