#pragma once
#include <string>
#include <memory>

#include "../common/vector2.hpp"
#include "../common/vector3.hpp"

namespace px
{
  class Shader
  {
  public:
    virtual ~Shader() {};

    virtual void use() const = 0;

    virtual void setUniform(const std::string &uniformName, int value) = 0;
    virtual void setUniform(const std::string &uniformName, float value) = 0;

    virtual void setUniform(const std::string &uniformName, Vector2 value) = 0;
    virtual void setUniform(const std::string &uniformName, Vector3 value) = 0;
  };
}