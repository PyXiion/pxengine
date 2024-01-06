#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace px
{
  enum class MouseButton : int {
    Left, Middle, Right,

    Count
  };

  static constexpr MouseButton getGlfwMouseButton(int btn) {
    switch (btn) {
      case GLFW_MOUSE_BUTTON_LEFT:
        return MouseButton::Left;
      case GLFW_MOUSE_BUTTON_MIDDLE:
        return MouseButton::Middle;
      case GLFW_MOUSE_BUTTON_RIGHT:
        return MouseButton::Right;
      default:
        return MouseButton::Count;
    }
  }

  enum KeyCode : int
  {
    KeyCode_Unknown = -1,
    KeyCode_Space = 32,
    KeyCode_Apostrophe = 39, /*'*/
    KeyCode_Comma = 44, /*,*/
    KeyCode_Minus = 45, /*-*/
    KeyCode_Period = 46,
    KeyCode_Slash = 47,

    KeyCode_0 = 48,
    KeyCode_1,
    KeyCode_2,
    KeyCode_3,
    KeyCode_4,
    KeyCode_5,
    KeyCode_6,
    KeyCode_7,
    KeyCode_8,
    KeyCode_9,

    KeyCode_Semicolon = 59,
    KeyCode_Equal = 61,

    KeyCode_A = 65,
    KeyCode_B,
    KeyCode_C,
    KeyCode_D,
    KeyCode_E,
    KeyCode_F,
    KeyCode_G,
    KeyCode_H,
    KeyCode_I,
    KeyCode_J,
    KeyCode_K,
    KeyCode_L,
    KeyCode_M,
    KeyCode_N,
    KeyCode_O,
    KeyCode_P,
    KeyCode_Q,
    KeyCode_R,
    KeyCode_S,
    KeyCode_T,
    KeyCode_U,
    KeyCode_V,
    KeyCode_W,
    KeyCode_X,
    KeyCode_Y,
    KeyCode_Z,

    KeyCode_LeftBracket = 91,
    KeyCode_Backslash,
    KeyCode_RightBracket,
    KeyCode_Accent = 96,

    KeyCode_Escape = 256,
    KeyCode_Enter,
    KeyCode_Tab,
    KeyCode_Backspace,
    KeyCode_Insert,
    KeyCode_Delete,
    KeyCode_Right,
    KeyCode_Left,
    KeyCode_Down,
    KeyCode_Up,
    KeyCode_PageUp,
    KeyCode_PageDown,
    KeyCode_Home,
    KeyCode_End,

    KeyCode_CapsLock = 280,
    KeyCode_ScrollLock,
    KeyCode_NumLock,
    KeyCode_PrintScreen,
    KeyCode_Pause,

    KeyCode_F1 = 290,
    KeyCode_F2,
    KeyCode_F3,
    KeyCode_F4,
    KeyCode_F5,
    KeyCode_F6,
    KeyCode_F7,
    KeyCode_F8,
    KeyCode_F9,
    KeyCode_F10,
    KeyCode_F11,
    KeyCode_F12,

    KeyCode_Keypad_0 = 320,
    KeyCode_Keypad_1,
    KeyCode_Keypad_2,
    KeyCode_Keypad_3,
    KeyCode_Keypad_4,
    KeyCode_Keypad_5,
    KeyCode_Keypad_6,
    KeyCode_Keypad_7,
    KeyCode_Keypad_8,
    KeyCode_Keypad_9,
    KeyCode_Keypad_Decimal,
    KeyCode_Keypad_Divide,
    KeyCode_Keypad_Multiply,
    KeyCode_Keypad_Subtract,
    KeyCode_Keypad_Add,
    KeyCode_Keypad_Enter,
    KeyCode_Keypad_Equal,

    KeyCode_LeftShift = 340,
    KeyCode_LeftControl,
    KeyCode_LeftAlt,
    KeyCode_LeftSuper,

    KeyCode_RightShift = 344,
    KeyCode_RightControl,
    KeyCode_RightAlt,
    KeyCode_RightSuper,

    KeyCode_Count
  };

  static constexpr KeyCode getGlfwKeyCode(int key) {
    return static_cast<KeyCode>(key);
  }

  namespace KeyModifiers {
    enum Enum {
      None     = 0,
      Shift    = 1 << 0,
      Control  = 1 << 1,
      Alt      = 1 << 2,
      Super    = 1 << 3,
      CapsLock = 1 << 4,
      NumLock  = 1 << 5
    };
  }
}
