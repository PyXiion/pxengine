#include "window.hpp"
#include <easy/profiler.h>
#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include "general.hpp"

#if BX_PLATFORM_LINUX
  #define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
  #define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
  #define GLFW_EXPOSE_NATIVE_COCOA
#endif
#include <GLFW/glfw3native.h>


px::Window::Window()
  : m_windowHandle(nullptr)
{

}

px::Window::Window(const std::string &window_name, int width, int height)
{
  create(window_name, width, height);
}

px::Window::~Window()
{
  close();
  glfwTerminate(); // TODO check is allowed to terminate
}


void px::Window::create(const std::string &window_name, int width, int height)
{
  EASY_BLOCK("px::Window::create");
  std::lock_guard lk(m_glfwMutex);

  if (!glfwInit())
  {
    throw std::runtime_error("Не удалось инициализировать GLFW :(");
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  m_windowHandle = glfwCreateWindow(width, height, window_name.c_str(), nullptr, nullptr);

  glfwSetWindowUserPointer(m_windowHandle, this);

  if (!m_windowHandle)
  {
    throw std::runtime_error("Не удалось создать окно :(");
  }

  glfwSwapInterval(0);

  // callbacks
  // window resize
  glfwSetWindowSizeCallback(m_windowHandle, [](GLFWwindow *window, int width, int height) {
    auto *self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    self->onResize(width, height);
  });

  // framebuffer resize
  glfwSetFramebufferSizeCallback(m_windowHandle, [](GLFWwindow *window, int width, int height) {
    auto *self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    self->onFramebufferResize(width, height);
  });

  // window close
  glfwSetWindowCloseCallback(m_windowHandle, [](GLFWwindow *window) {
    auto *self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    self->onClose();
  });

  // window key pressed/released
  glfwSetKeyCallback(m_windowHandle, [](GLFWwindow *window, int key, int scancode, int action, int _mods) {
    auto *self = static_cast<Window*>(glfwGetWindowUserPointer(window));

    auto keycode = static_cast<KeyCode>(key);
    auto mods = static_cast<KeyModifiers::Enum>(_mods);

    if (action == GLFW_PRESS) {
      self->onKeyPressed(keycode, mods);
      self->onKey(keycode, true, mods);
    }
    else if (action == GLFW_RELEASE) {
      self->onKeyReleased(keycode, mods);
      self->onKey(keycode, false, mods);
    }

  });

  // cursor moved
  glfwSetCursorPosCallback(m_windowHandle, [](GLFWwindow *window, double x, double y) {
    auto *self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    self->m_lastMousePosition = {x, y};
    self->onMouseMoved(float(x), float(y));
  });

  // mouse click
  glfwSetMouseButtonCallback(m_windowHandle, [](GLFWwindow *window, int button, int action, int mods) {
    auto *self = static_cast<Window*>(glfwGetWindowUserPointer(window));

    float x, y;
    std::tie(x, y) = self->m_lastMousePosition;
    self->onMouseClicked(x, y, getGlfwMouseButton(button), action == GLFW_PRESS);
  });

  // mouse scroll
  glfwSetScrollCallback(m_windowHandle, [](GLFWwindow *window, double xoffset, double yoffset) {
    auto *self = static_cast<Window*>(glfwGetWindowUserPointer(window));

    self->onHorizontalScroll(float(xoffset));
    self->onVerticalScroll(float(yoffset));
    self->onScroll(float(xoffset), float(yoffset));
  });

  // character input
  glfwSetCharCallback(m_windowHandle, [](GLFWwindow *window, unsigned int c) {
    auto *self = static_cast<Window*>(glfwGetWindowUserPointer(window));

    self->onInput(c);
  });


}

void px::Window::close()
{
  if (m_windowHandle) {
    glfwDestroyWindow(m_windowHandle);
    m_windowHandle = nullptr;
  }
}

bool px::Window::isShouldClose() const
{
  return glfwWindowShouldClose(m_windowHandle);
}

bool px::Window::isKeyPressed(int key) const
{
  return glfwGetKey(m_windowHandle, key) == GLFW_PRESS;
}

void px::Window::pollEvents()
{
  EASY_BLOCK("px::Window::pollEvents", profiler::colors::Cyan300);
  glfwPollEvents();
}

GLFWwindow *px::Window::getHandle()
{
  return m_windowHandle;
}

std::pair<int, int> px::Window::size() {
  int width, height;
  glfwGetWindowSize(getHandle(), &width, &height);
  return {width, height};
}

void *px::Window::getDisplayHandle() {
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
  return glfwGetX11Display();
#else
  return nullptr;
#endif
}

void *px::Window::getNativeHandle() {
#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
  return reinterpret_cast<void*>(glfwGetX11Window(getHandle()));
#elif BX_PLATFORM_OSX
  return reinterpret_cast<void*>(glfwGetCocoaWindow(getHandle()));
#elif BX_PLATFORM_WINDOWS
  return reinterpret_cast<void*>(glfwGetWin32Window(getHandle()));
#endif
}

float px::Window::getAspect() {
  int width, height;
  std::tie(width, height) = size();
  return static_cast<float>(width) / static_cast<float>(height);
}
