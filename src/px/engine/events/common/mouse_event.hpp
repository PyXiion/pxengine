// Copyright (c) 2023.

//
// Created by pyxiion on 27.11.23.
//

#ifndef PX_ENGINE_MOUSE_EVENT_HPP
#define PX_ENGINE_MOUSE_EVENT_HPP
#include "../event.hpp"
#include "../event_fabric.hpp"

  namespace px {
    struct MouseEvent final : BaseEvent<MouseEvent, "core.event.mouse"> {
      MouseEvent(float x, float y)
        : x(x), y(y) {}
      ~MouseEvent() override = default;

      float x{};
      float y{};
    };

    typedef CustomEventPtr<MouseEvent> MouseEventPtr;
  } // px

#endif //PX_ENGINE_MOUSE_EVENT_HPP
