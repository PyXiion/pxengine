#include "controls.hpp"
#include "../engine.hpp"

px::Controls::Controls(px::Window &window)
{
  listen(window.onKey, [this](auto key, auto down, auto mods) {
    processKey(key, down);
  });
}

float px::Controls::getAxis(px::ControlAxis::Enum axis) const {
  std::shared_lock lk(m_axesMutex);
  return m_axes[axis];
}

void px::Controls::processKey(px::KeyCode key, bool down) {
  auto setAxis = [this, &down](ControlAxis::Enum _axis, float shouldBe) {
    auto &axis = m_axes[_axis];
    if (axis == shouldBe && not down) {
      axis = 0;
    } else if (down) {
      axis = shouldBe;
    }
  };

  std::unique_lock lk(m_axesMutex);
  switch (key) {
    case KeyCode_W:
      setAxis(ControlAxis::Vertical, 1.0f);
      break;
    case KeyCode_S:
      setAxis(ControlAxis::Vertical, -1.0f);
      break;

    case KeyCode_D:
      setAxis(ControlAxis::Horizontal, 1.0f);
      break;
    case KeyCode_A:
      setAxis(ControlAxis::Horizontal, -1.0f);
      break;

    case KeyCode_Space:
      setAxis(ControlAxis::Jump, 1.0f);
      break;

    default:
      break;
  }
}
