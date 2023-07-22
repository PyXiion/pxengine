#pragma once
#include <vector>
#include "../vertex_array.hpp"

namespace px::gl
{

  class VertexArray final : public px::VertexArray
  {
  private:
    using BufferHandle = unsigned int;

  public:
    VertexArray(const std::vector<float> vertices, const std::vector<VertexArrayAttribute> &attributes, DrawType drawType, PrimitiveType primitiveType);
    ~VertexArray();

    void bind() const override;
    void unbind() const override;

    void draw() const override;

  private:
    BufferHandle m_vao;
    BufferHandle m_vbo;

    int m_vertexCount;
    int m_primitiveType;
  };
}
