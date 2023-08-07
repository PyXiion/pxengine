#pragma once
#include <string>
#include <memory>
#include <glm/glm.hpp>
#include "shader.hpp"
#include "vertex_array.hpp"

namespace px
{
  /// @brief Абстрактный класс для рендера. Может использовать OpenGL и Vulkan.
  class Renderer
  {
  public:
    virtual ~Renderer() = default;

    virtual void setProjectionMatrix(glm::mat4 projection_matrix) = 0;
    virtual void setViewMatrix(glm::mat4 view_matrix) = 0;

    virtual void setViewportSize(int left, int top, int width, int height) = 0;

    virtual std::shared_ptr<Shader> createShader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath) = 0;
    virtual std::shared_ptr<VertexArray> createVertexArray(const std::vector<float> vertices, const std::vector<VertexArrayAttribute> &attributes, DrawType drawType, PrimitiveType primitiveType) = 0;

    virtual void useContext() = 0;

    /// @brief Очищает экран.
    /// @param color Цвет.
    virtual void clear(Vector3 color = Vector3(0.0f, 0.0f, 0.0f)) = 0;

    /// @brief Переключение буфера.
    virtual void swapBuffers() = 0;

    virtual void initImGui() = 0;
    virtual void beginImGuiFrame() = 0;
    virtual void endImGuiFrame() = 0;
    virtual void renderImGui() = 0;
  };
}
