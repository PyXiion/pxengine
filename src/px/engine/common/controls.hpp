#pragma once
#include <array>
#include "../system/window.hpp"
#include "../events/event_listener.hpp"

namespace px {
  class Window;

  namespace ControlAxis {
    enum Enum {
      Vertical,
      Horizontal,

      Jump,

      Count
    };
  }


  class Controls : private EventListener {
  public:
    explicit Controls(Window &engine);

    [[nodiscard]] float getAxis(ControlAxis::Enum axis) const;

  private:
    mutable std::shared_mutex m_axesMutex;
    std::array<float, ControlAxis::Count> m_axes;

    void processKey(KeyCode key, bool down);
  };
} // namespace px
