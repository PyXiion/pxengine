#include "resource_manager.hpp"
#include <filesystem>
#include <px/string_utils.hpp>
#include <px/visit_overloaded.hpp>
#include <utility>
#include <fstream>
#include "../common/utils.hpp"
#include "../engine.hpp"
#include "../utils/bgfx_utils.hpp"

namespace fs = std::filesystem;

px::ResourceManager::ResourceManager(Engine &engine, std::string rootDir)
  : m_engine(engine)
  , m_rootDir(std::move(rootDir))
{

}

px::ResourceManager::~ResourceManager() {
}

px::ShaderPtr px::ResourceManager::loadShader(const std::string &vsName, const std::string &fsName, bool reload) {
  EASY_BLOCK("px::ResourceManager::loadShader")
  std::string key = vsName + "," + fsName;

  Resource &resource = (*this)[key];
  if (not reload and std::holds_alternative<ShaderPtr>(resource)) {
    return std::get<ShaderPtr>(resource);
  }

  bgfx::ShaderHandle vsh = loadShaderFile(vsName, reload);
  bgfx::ShaderHandle fsh = BGFX_INVALID_HANDLE;
  if (!fsName.empty())
  {
    fsh = loadShaderFile(fsName, reload);
  }

  auto shader = makeShader(vsh, fsh);
  resource = shader;
  return shader;
}

bgfx::ShaderHandle px::ResourceManager::loadShaderFile(const std::string &filename, bool reload) {
  EASY_BLOCK("px::ResourceManager::loadShaderFile")
  Resource &resource = (*this)[filename];
  if (not reload and std::holds_alternative<BgfxUniqueShaderHandle>(resource)) {
    return static_cast<bgfx::ShaderHandle>(std::get<BgfxUniqueShaderHandle>(resource));
  }

  // Create a new shader if not cached
  std::vector<std::string> shaderPath = ResourceManager::parseResourcePath(filename);
  std::string shaderType;

  switch (bgfx::getRendererType()) {
    case bgfx::RendererType::Noop:
    case bgfx::RendererType::Direct3D9:  shaderType = "dx9"; break;
    case bgfx::RendererType::Direct3D11:
    case bgfx::RendererType::Direct3D12: shaderType = "dx11";  break;
    case bgfx::RendererType::Agc:
    case bgfx::RendererType::Gnm:        shaderType = "pssl";  break;
    case bgfx::RendererType::Metal:      shaderType = "metal"; break;
    case bgfx::RendererType::Nvn:        shaderType = "nvn";   break;
    case bgfx::RendererType::OpenGL:     shaderType = "glsl";  break;
    case bgfx::RendererType::OpenGLES:   shaderType = "essl";  break;
    case bgfx::RendererType::Vulkan:
    case bgfx::RendererType::WebGPU:     shaderType = "spirv"; break;

    default:
      throw std::runtime_error("Unknown renderer type.");
  }
  shaderPath.insert(shaderPath.end() - 1, shaderType);
  fs::path path = m_rootDir;
  for (const auto& i : shaderPath)
    path /= i;

  path.replace_extension(".bin");

  bgfx::ShaderHandle handle = bgfx::createShader(utils::bgfx::stringToMemory(px::io::loadFileToString(path)));
  bgfx::setName(handle, filename.c_str());

  resource = BgfxUniqueShaderHandle(handle);
  return handle;
}

  EASY_BLOCK("px::ResourceManager::loadTexture")
px::ResourceManager::RawType px::ResourceManager::loadRawFile(const std::string &_path, bool reload) {
  EASY_BLOCK("px::ResourceManager::loadRawFile")
  fs::path path = absolutePath(_path);

  Resource &resource = (*this)[_path];
  if (not reload and std::holds_alternative<RawTypeHandler>(resource)) {
    auto &handle = std::get<RawTypeHandler>(resource);
    return RawType(handle.data(), handle.size());
  }

  std::ifstream ifs(path, std::ios::in | std::ios::binary);
  if (not ifs.is_open()) {
    throw std::runtime_error("File does not exist.");
  }
  px::MemoryStream ms;
  ifs >> ms;
  ifs.close();

  RawType raw(ms.data(), ms.size());
  resource = std::move(ms);

  return raw;
}

  EASY_BLOCK("px::ResourceManager::loadLocalization")
std::string px::ResourceManager::absolutePath(const std::string &_path)
{
  fs::path path = _path;
  if (path.is_absolute())
  {
    return path;
  }

  return fs::absolute(fs::path(m_rootDir) / path);
}

std::vector<std::string> px::ResourceManager::parseResourcePath(const std::string &path) {
  return px::split(path, ".");
}

px::ResourceManager::Resource &px::ResourceManager::operator[](const std::string &key) {
  return m_cached[key];
}
