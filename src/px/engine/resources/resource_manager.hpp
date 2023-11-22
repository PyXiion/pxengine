#pragma once
#include <string>
#include <unordered_map>
#include <bgfx/bgfx.h>
#include <vector>
#include <variant>
#include <px/hash_utils.hpp>
#include <px/raw_memory.hpp>
#include <px/memory_stream.hpp>
#include "bgfx_handle.hpp"

namespace px
{ 
  class Engine;
  class ResourceManager
  {
  public:
    struct None {};
    using RawType = px::RawConstMemory<char>;
    using RawTypeHandler = px::MemoryStream;
    using Resource = std::variant<
        None,
        BgfxUniqueVertexBufferHandle, BgfxUniqueIndexBufferHandle,
        BgfxUniqueShaderHandle, BgfxUniqueProgramHandle,
        RawTypeHandler>;

  public:
    ResourceManager(Engine &engine, std::string rootDir);
    ~ResourceManager();

    Resource &operator[](const std::string &key);

    /// Loads a shader from a file.
    /// \param filename The name of the file without a pre-directory. It will be determined automatically depending on the current graphics API.
    /// \return A bgfx shader handle
    bgfx::ShaderHandle loadShaderFile(const std::string &filename, bool reload = false);

    /// Loads a shader program from files.
    /// \param vsName The file name of the vertex shader.
    /// \param fsName The file name of the fragment shader.
    /// \return A bgfx program handle
    ShaderPtr loadShader(const std::string &vsName, const std::string &fsName, bool reload = false);

    const Localization &loadLocalization(const std::string &localization, bool reload = false);

    TexturePtr loadTexture(const std::string &texture, bool reload = false);

    RawType loadRawFile(const std::string &path, bool reload = false);

  private:
    Engine &m_engine;

    std::string m_rootDir;

    std::unordered_map<std::string, Resource> m_cached;
    /// Makes the path absolute depending on the root directory of the resources.
    /// \param _path A path.
    /// \return An absolute path.
    std::string absolutePath(const std::string &_path);

    static std::vector<std::string> parseResourcePath(const std::string &path);
  };
} // namespace px
