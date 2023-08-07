#pragma once
#include "../renderer.hpp"
#include "../../system/window.hpp"

namespace px::gl
{
  /// @brief Реализация рендера для OpenGL
  class Renderer final : public px::Renderer
  {
  public:
    Renderer(Window &window);
    ~Renderer();

    void setProjectionMatrix(glm::mat4 projection_matrix) override;
    void setViewMatrix(glm::mat4 view_matrix) override;

    void setViewportSize(int left, int top, int width, int height) override;

    std::shared_ptr<px::Shader> createShader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath) override;
    std::shared_ptr<px::VertexArray> createVertexArray(const std::vector<float> vertices, const std::vector<VertexArrayAttribute> &attributes, DrawType drawType, PrimitiveType primitiveType) override;

    void useContext() override;

    void clear(px::Vector3 color) override;
    void swapBuffers() override;

    void initImGui() override;

    void beginImGuiFrame() override;
    void endImGuiFrame() override;
    void renderImGui() override;

  private:
    Window &m_window;
    
    /// @brief Закэшировано, чтобы каждый раз не использовать геттер.
    GLFWwindow *const m_window_handle;
  };
}
