#pragma once
#include <string>
#include <unordered_map>
#include <bgfx/bgfx.h>

namespace px
{ 
  class Engine;
  class ResourceManager
  {
  public:
    ResourceManager(Engine &engine, const std::string &rootDir);

    /// Loads a shader from a file.
    /// \param filename The name of the file without a pre-directory. It will be determined automatically depending on the current graphics API.
    /// \return A bgfx shader handle
    bgfx::ShaderHandle loadShader(const std::string &filename);

    /// Loads a shader program from files.
    /// \param vsName The file name of the vertex shader.
    /// \param fsName The file name of the fragment shader.
    /// \return A bgfx program handle
    bgfx::ProgramHandle loadProgram(const std::string &vsName, const std::string &fsName);

  private:
    Engine &m_engine;
    std::string m_rootDir;

    /// Makes the path absolute depending on the root directory of the resources.
    /// \param path A path.
    /// \return An absolute path.
    std::string normalizePath(const std::string &path);
  };
} // namespace px
