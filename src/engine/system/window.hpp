#pragma once
#include <string>
#include <eventpp/callbacklist.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "keycodes.hpp"

namespace px {
  class Window {
  public:
    Window();
    Window(const std::string &window_name,
           int width, int height);
    ~Window();

    void create(const std::string &window_name, int width, int height);
    void close();

    bool isShouldClose() const;
    bool isKeyPressed(int key) const;

    void pollEvents();

    GLFWwindow *getHandle();

    eventpp::CallbackList<void()> onClose;
    eventpp::CallbackList<void(int width, int height)> onResize;
    eventpp::CallbackList<void(int width, int height)> onFramebufferResize;

    eventpp::CallbackList<void(KeyCode keycode)> onKeyPressed;
    eventpp::CallbackList<void(KeyCode keycode)> onKeyReleased;

    eventpp::CallbackList<void(double x, double y)> onMouseMoved;

  private:
    GLFWwindow *m_windowHandle;
  };
}
