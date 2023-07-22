#include "window.hpp"
#include <easy/profiler.h>
#include "../system/general.hpp"

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
  if (m_windowHandle)
    glfwDestroyWindow(m_windowHandle);

  glfwTerminate(); // TODO check is allowed to terminate
}


void px::Window::create(const std::string &window_name, int width, int height)
{
  EASY_BLOCK("px::Window::create");
  glfwInit();
  // TODO make support for a choice between Vulkan and OpenGL
  // so far, it's hardcoded
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // For MacOS
#ifdef PX_OS_MACOS
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
#endif

  m_windowHandle = glfwCreateWindow(width, height, window_name.c_str(), nullptr, nullptr);

  glfwSetWindowUserPointer(m_windowHandle, this);

  if (!m_windowHandle)
  {
    // TODO throw error
  }

  // callbacks
  // window resize
  glfwSetWindowSizeCallback(m_windowHandle, [](GLFWwindow *window, int width, int height) {
    Window *self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    self->onFramebufferResize(width, height);
  });

  // framebuffer resize
  glfwSetFramebufferSizeCallback(m_windowHandle, [](GLFWwindow *window, int width, int height) {
    Window *self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    self->onResize(width, height);
  });

  // window close
  glfwSetWindowCloseCallback(m_windowHandle, [](GLFWwindow *window) {
    Window *self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    self->onClose();
  });

  // window key pressed/released
  glfwSetKeyCallback(m_windowHandle, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
    Window *self = static_cast<Window*>(glfwGetWindowUserPointer(window));

    KeyCode keycode = static_cast<KeyCode>(key);

    if (action == GLFW_PRESS)
      self->onKeyPressed(keycode);
    else if (action == GLFW_RELEASE)
      self->onKeyReleased(keycode);
  });

  // cursor moved
  glfwSetCursorPosCallback(m_windowHandle, [](GLFWwindow *window, double x, double y) {
    Window *self = static_cast<Window*>(glfwGetWindowUserPointer(window));
    self->onMouseMoved(x, y);
  });
}

void px::Window::close()
{
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
