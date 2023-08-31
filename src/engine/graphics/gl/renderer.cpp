#include "renderer.hpp"

#include <imgui/backend/imgui_impl_opengl3.h>
#include <imgui/backend/imgui_impl_glfw.h>
#include <glad/glad.h>
#include <easy/profiler.h>

#include "shader.hpp"
#include "vertex_array.hpp"
#include "../../common/utils.hpp"
#include "../../general.hpp"

px::gl::Renderer::Renderer(Window &window)
  : m_window(window)
  , m_window_handle(window.getHandle())
{
  useContext();

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
  {
    throw std::runtime_error("Не удалось инициализировать GLAD.");
  }
}

px::gl::Renderer::~Renderer()
{

}

void px::gl::Renderer::setProjectionMatrix(glm::mat4 projection_matrix)
{
}

void px::gl::Renderer::setViewMatrix(glm::mat4 view_matrix)
{
}

void px::gl::Renderer::setViewportSize(int left, int top, int width, int height)
{
  glViewport(left, top, width, height);
}

std::shared_ptr<px::Shader> px::gl::Renderer::createShader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath)
{
  return std::make_shared<gl::Shader>(io::loadFileToString(vertexShaderPath), io::loadFileToString(fragmentShaderPath));
}

std::shared_ptr<px::VertexArray> 
px::gl::Renderer::createVertexArray(
  const std::vector<float> vertices,
  const std::vector<px::VertexArrayAttribute> &attributes,
  px::DrawType drawType,
  px::PrimitiveType primitiveType)
{
  return std::make_shared<gl::VertexArray>(vertices, attributes, drawType, primitiveType);
}


void px::gl::Renderer::useContext()
{
  EASY_BLOCK("px::gl::Rendered::useContext");
  glfwMakeContextCurrent(m_window.getHandle());
}

void px::gl::Renderer::clear(px::Vector3 color)
{
  glClearColor(color.x, color.y, color.z, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}


void px::gl::Renderer::swapBuffers()
{
  EASY_BLOCK("px::gl::Rendered::swapBuffers");
  glfwSwapBuffers(m_window_handle);
}

void px::gl::Renderer::initImGui()
{
  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(m_window_handle, true);
  ImGui_ImplOpenGL3_Init(px::info::glslVersion.data());
}

void px::gl::Renderer::beginImGuiFrame()
{
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void px::gl::Renderer::endImGuiFrame()
{
  ImGui::EndFrame();
}

void px::gl::Renderer::renderImGui()
{
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
