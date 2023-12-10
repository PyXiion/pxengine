#pragma once
#include <array>
#include <shared_mutex>
#include "../system/window.hpp"
#include "../events/event_listener.hpp"
#include "px/engine/math/vector.hpp"

namespace px {
  class Window;

  namespace ControlAxis {
    enum Enum {
      Vertical,
      Horizontal,

      Jump,

      MouseX,
      MouseY,

      Count
    };
  }

  class Engine;

  class Controls : private EventListener {
  public:
    explicit Controls(Engine &engine, Window &window);

    [[nodiscard]] float getAxis(ControlAxis::Enum axis) const;

  private:
    mutable std::shared_mutex m_axesMutex;
    std::array<float, ControlAxis::Count> m_axes;

    void processKey(KeyCode key, bool down);

    Vector2 m_lastMousePos;
    bool m_firstMouseMovement;
    void processMouse(float x, float y);
  };
} // namespace px
