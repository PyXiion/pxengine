#pragma once
#include <vector>
#include <cstdint>

namespace px
{
  enum VAAttributeType
  {
    VAAttributeType_Float,
    VAAttributeType_Int
  };
  struct VertexArrayAttribute
  {
    int location;
    int size;
    VAAttributeType type;
    bool normalized = false;
    int stride;
    intptr_t offset;

    VertexArrayAttribute(int location, int size, VAAttributeType type, bool normalized, int stride, intptr_t offset)
      : location(location)
      , size(size)
      , type(type)
      , normalized(normalized)
      , stride(stride)
      , offset(offset)
    {}
    VertexArrayAttribute(int location, int size, int stride, intptr_t offset)
      : location(location)
      , size(size)
      , type(VAAttributeType_Float)
      , normalized(false)
      , stride(stride)
      , offset(offset)
    {}
  };

  enum DrawType
  {
    DrawType_Static,
    DrawType_Dynamic,
    DrawType_Render
  };
  enum PrimitiveType
  {
    PrimitiveType_Triangles,
    PrimitiveType_Squares,
    PrimitiveType_Dots
  };

  class VertexArray
  {
  public:
    virtual ~VertexArray() {}

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual void draw() const = 0;
  };
}
