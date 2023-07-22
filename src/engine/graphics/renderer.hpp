#pragma once
#include <string>
#include <memory>
#include "shader.hpp"
#include "vertex_array.hpp"
#include "../common/matrix4x4.hpp"

namespace px
{
  class Renderer
  {
  public:
    virtual ~Renderer() {};

    virtual void setProjectionMatrix(Matrix4x4 projection_matrix) = 0;
    virtual void setViewMatrix(Matrix4x4 view_matrix) = 0;

    virtual void setViewportSize(int left, int top, int width, int height) = 0;

    virtual std::shared_ptr<Shader> createShader(const std::string &vertexShaderPath, const std::string &fragmentShaderPath) = 0;
    virtual std::shared_ptr<VertexArray> createVertexArray(const std::vector<float> vertices, const std::vector<VertexArrayAttribute> &attributes, DrawType drawType, PrimitiveType primitiveType) = 0;

    virtual void useContext() = 0;

    virtual void clear(Vector3 color = Vector3(0.0f, 0.0f, 0.0f)) = 0;
    virtual void swapBuffers() = 0;
  };
}
