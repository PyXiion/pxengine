#include "vertex_array.hpp"
#include <glad/glad.h>

px::gl::VertexArray::VertexArray(const std::vector<float> vertices, const std::vector<VertexArrayAttribute> &attributes, DrawType drawType, PrimitiveType primitiveType)
{
  glGenBuffers(1, &m_vbo);
  glGenVertexArrays(1, &m_vao);

  glBindVertexArray(m_vao);

  glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW); // TODO choice draw type

  int vertexSize = 0;
  for (auto attribute : attributes)
  {
    GLenum type;
    switch (attribute.type)
    {
      default:
      case VAAttributeType_Float:
        type = GL_FLOAT;
        break;
      case VAAttributeType_Int:
        type = GL_INT;
        break;
    }

    glVertexAttribPointer(attribute.location, attribute.size, type, attribute.normalized, attribute.stride, reinterpret_cast<void*>(attribute.offset));
    glEnableVertexAttribArray(attribute.location);

    vertexSize += attribute.size;
  }

  m_vertexCount = vertices.size() / vertexSize; // получить количество вершит, так как каждая вершина состоит из аттрибутов

  switch (primitiveType) // TODO добавить больше типов
  {
    case PrimitiveType_Triangles:
    default:
      m_primitiveType = GL_TRIANGLES;
      break;
  }

  unbind();
}

px::gl::VertexArray::~VertexArray()
{
}

void px::gl::VertexArray::bind() const
{
  glBindVertexArray(m_vao);
}

void px::gl::VertexArray::unbind() const
{
  glBindVertexArray(0);
}

void px::gl::VertexArray::draw() const
{
  glDrawArrays(m_primitiveType, 0, m_vertexCount);
}
