#include "controls.hpp"
#include "../engine.hpp"

px::Controls::Controls(px::Engine &engine, px::Window &window)
  : m_lastMousePos()
  , m_axes()
  , m_firstMouseMovement(true) {
  listen(window.onKey, [this](auto key, auto down, auto mods) {
    processKey(key, down);
  });

  engine.onUpdate.append([this, &window](float dt) {
    double x,y;
    glfwGetCursorPos(window.getHandle(), &x, &y);
    processMouse(static_cast<float>(x), static_cast<float>(y));
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

void px::Controls::processMouse(float x, float y) {
  if (not m_firstMouseMovement) {
    std::unique_lock lk(m_axesMutex);
    m_axes[ControlAxis::MouseX] = x - m_lastMousePos.x;
    m_axes[ControlAxis::MouseY] = y - m_lastMousePos.y;
  }
  m_lastMousePos = {x, y};

  m_firstMouseMovement = false;
}
