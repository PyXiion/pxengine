#include "shader.hpp"
#include <iostream>

#include <glad/glad.h>
#include <fmt/format.h>

px::gl::Shader::Shader(const std::string &vertexSource, const std::string &fragmentSource)
{
  m_vertexHandle = glCreateShader(GL_VERTEX_SHADER);
  const char *vertexSourcePtr = vertexSource.c_str();
  glShaderSource(m_vertexHandle, 1, &vertexSourcePtr, nullptr);
  glCompileShader(m_vertexHandle);
  if (!checkShaderCompileSuccess(m_vertexHandle))
  {
    throw std::runtime_error("Не удалось скомпилировать вершинный шейдер.");
  }

  m_fragmentHandle = glCreateShader(GL_FRAGMENT_SHADER);
  const char *fragmentSourcePtr = fragmentSource.c_str();
  glShaderSource(m_fragmentHandle, 1, &fragmentSourcePtr, nullptr);
  glCompileShader(m_fragmentHandle);
  if (!checkShaderCompileSuccess(m_fragmentHandle))
  {
    throw std::runtime_error("Не удалось скомпилировать фрагментный шейдер.");
  }

  m_programHandle = glCreateProgram();
  glAttachShader(m_programHandle, m_vertexHandle);
  glAttachShader(m_programHandle, m_fragmentHandle);
  glLinkProgram(m_programHandle);

  if (!checkProgramCompileSuccess(m_programHandle))
  {
    throw std::runtime_error("Не удалось слинковать программу шейдера.");
  }
}

px::gl::Shader::~Shader()
{
  glDeleteShader(m_vertexHandle);
  glDeleteShader(m_fragmentHandle);
  glDeleteProgram(m_programHandle);
}

void px::gl::Shader::setUniform(const std::string& uniformName, int value)
{
  use();
  glUniform1i(getUniform(uniformName), value);
}

void px::gl::Shader::setUniform(const std::string& uniformName, float value)
{
  use();
  glUniform1f(getUniform(uniformName), value);
}

void px::gl::Shader::setUniform(const std::string &uniformName, px::Vector2 value)
{
  use();
  glUniform2f(getUniform(uniformName), value.x, value.y);
}

void px::gl::Shader::setUniform(const std::string &uniformName, px::Vector3 value)
{
  use();
  glUniform3f(getUniform(uniformName), value.x, value.y, value.z);
}

px::gl::Shader::UniformHandle px::gl::Shader::getUniform(const std::string &uniformName)
{
  UniformHandle handle = glGetUniformLocation(m_programHandle, uniformName.c_str());
  if (handle == -1)
  {
    throw std::runtime_error(fmt::format("Uniform {} не найден.", uniformName));
  }
  return handle;
}


void px::gl::Shader::use() const
{
  glUseProgram(m_programHandle);
}


bool px::gl::Shader::checkShaderCompileSuccess(px::gl::Shader::ShaderHandle handle)
{
  int success;
  char infoLog[512];
  glGetShaderiv(handle, GL_COMPILE_STATUS, &success);

  if (!success)
  {
    glad_glGetShaderInfoLog(handle, sizeof(infoLog), nullptr, infoLog);
    std::cout << "[GLSL] [Ошибка] Ошибка компиляции: " << infoLog << std::endl;
    return false;
  }
  return true;
}
bool px::gl::Shader::checkProgramCompileSuccess(px::gl::Shader::ProgramHandle handle) // то же самое: просто заменить Shader на Program
{
  int success;
  char infoLog[512];
  glGetProgramiv(handle, GL_LINK_STATUS, &success);

  if (!success)
  {
    glad_glGetProgramInfoLog(handle, sizeof(infoLog), nullptr, infoLog);
    std::cout << "[GLSL] [Ошибка] Ошибка компиляции: " << infoLog << std::endl;
    return false;
  }
  return true;
}
