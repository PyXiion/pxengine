#pragma once
#include <string>
#include <mutex>
#include <eventpp/callbacklist.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "keycodes.hpp"

namespace px {
  class Window {
  private:
    using OnResizeCallbackList = eventpp::CallbackList<void(int width, int height)>;
    using OnKeyCallbackList = eventpp::CallbackList<void(KeyCode keycode)>;

  public:
    using OnResizeHandle = OnResizeCallbackList::Handle;
    using OnKeyHandle = OnKeyCallbackList::Handle;


  public:
    Window();
    Window(const std::string &window_name,
           int width, int height);
    ~Window();

    void create(const std::string &window_name, int width, int height);
    void close();

    bool isShouldClose() const;
    bool isKeyPressed(int key) const;

    std::pair<int, int> size();
    float getAspect();

    void *getDisplayHandle();
    void *getNativeHandle();

    void pollEvents();

    GLFWwindow *getHandle();

    eventpp::CallbackList<void()> onClose;
    OnResizeCallbackList onResize;
    OnResizeCallbackList onFramebufferResize;

    OnKeyCallbackList onKeyPressed;
    OnKeyCallbackList onKeyReleased;

    eventpp::CallbackList<void(double x, double y)> onMouseMoved;

  private:
    GLFWwindow *m_windowHandle;

    std::mutex m_glfwMutex;
  };
}
