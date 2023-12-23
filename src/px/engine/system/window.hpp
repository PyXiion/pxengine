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

    using OnKeyCallbackList = eventpp::CallbackList<void(KeyCode keycode, bool down, KeyModifiers::Enum mods)>;
    using OnKeyPressedCallbackList = eventpp::CallbackList<void(KeyCode keycode, KeyModifiers::Enum mods)>;
    using OnKeyReleasedCallbackList = OnKeyPressedCallbackList;

    using OnMouseMovedCallbackList = eventpp::CallbackList<void(float x, float y)>;
    using OnMouseClickedCallbackList = eventpp::CallbackList<void(float x, float y, MouseButton button, bool down)>;

    using OnScrollVerticalCallbackList = eventpp::CallbackList<void(float scroll)>;
    using OnScrollHorizontalCallbackList = OnScrollVerticalCallbackList;
    using OnScrollCallbackList = eventpp::CallbackList<void(float scrollX, float scrollY)>;

    using OnCharacterInputCallbackList = eventpp::CallbackList<void(unsigned int character)>;

  public:
    using OnKeyHandle = OnKeyPressedCallbackList::Handle;


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

    void setCursorHidden(bool hidden);

    void hideCursor();

    void showCursor();

    bool isCursorHidden() const;

    static void *getDisplayHandle();
    void *getNativeHandle();

    static void pollEvents();

    GLFWwindow *getHandle();

    eventpp::CallbackList<void()> onClose;
    OnResizeCallbackList onResize;
    OnResizeCallbackList onFramebufferResize;

    OnKeyCallbackList onKey;
    OnKeyPressedCallbackList onKeyPressed;
    OnKeyReleasedCallbackList onKeyReleased;

    OnMouseMovedCallbackList onMouseMoved;
    OnMouseClickedCallbackList onMouseClicked;

    OnScrollVerticalCallbackList onVerticalScroll;
    OnScrollHorizontalCallbackList onHorizontalScroll;
    OnScrollCallbackList onScroll;

    OnCharacterInputCallbackList onInput;

  private:
    GLFWwindow *m_windowHandle{};

    std::mutex m_glfwMutex;

    std::pair<float, float> m_lastMousePosition;
  };
}
