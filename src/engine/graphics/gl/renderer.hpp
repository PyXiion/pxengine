#pragma once
#include "../renderer.hpp"
#include "../../system/window.hpp"

namespace px::gl
{
  class Renderer final : public px::Renderer
  {
  public:
    Renderer(Window &window);
    ~Renderer();

    void setProjectionMatrix(glm::mat4 projection_matrix) override;
    void setViewMatrix(glm::mat4 view_matrix) override;

    void setViewportSize(int left, int top, int width, int height) override;

    std::shared_ptr<Shader> createShader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath) override;
    std::shared_ptr<VertexArray> createVertexArray(const std::vector<float> vertices, const std::vector<VertexArrayAttribute> &attributes, DrawType drawType, PrimitiveType primitiveType) override;

    void useContext() override;

    void clear(px::Vector3 color) override;
    void swapBuffers() override;

  private:
    Window &m_window;
    GLFWwindow *const m_window_handle;
  };
}
