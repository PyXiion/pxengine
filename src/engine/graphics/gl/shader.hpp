#pragma once
#include "../shader.hpp"

namespace px::gl
{
  class Shader final : public px::Shader
  {
    using ProgramHandle = unsigned int;
    using ShaderHandle = unsigned int;
    using UniformHandle = unsigned int;

  public:
    Shader(const std::string &vertexSource, const std::string &fragmentSource);
    ~Shader();

    void use() const override;

    void setUniform(const std::string &uniformName, int value) override;
    void setUniform(const std::string &uniformName, float value) override;

    void setUniform(const std::string &uniformName, px::Vector2 value) override;
    void setUniform(const std::string &uniformName, px::Vector3 value) override;

  private:
    ProgramHandle m_programHandle;
    ShaderHandle m_vertexHandle;
    ShaderHandle m_fragmentHandle;

    /*
     * Returns OpenGL handler to uniform
     */
    UniformHandle getUniform(const std::string &uniformName);

    /*
     * Returns true if successful
     */
    static bool checkShaderCompileSuccess(ShaderHandle handle);
    /*
     * Returns true if successful
     */
    static bool checkProgramCompileSuccess(ProgramHandle handle);
  };
}
