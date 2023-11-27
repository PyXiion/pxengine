// Copyright (c) 2023.

//
// Created by pyxiion on 27.11.23.
//

#ifndef PX_ENGINE_MOUSE_EVENT_HPP
#define PX_ENGINE_MOUSE_EVENT_HPP
#include "../event.hpp"
#include "../event_fabric.hpp"

  namespace px {
    struct MouseEvent final : public BaseEvent<MouseEvent, "core.event.mouse"> {
      ~MouseEvent() override = default;

      float x{};
      float y{};
    };
  } // px

#endif //PX_ENGINE_MOUSE_EVENT_HPP
