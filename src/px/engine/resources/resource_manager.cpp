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
  CLOG(INFO, "PXEngine") << "Loading new shader:";
  CLOG(INFO, "PXEngine") << "\t" << vsName;
  CLOG(INFO, "PXEngine") << "\t" << fsName;

  bgfx::ShaderHandle vsh = loadShaderFile(vsName, reload);
  bgfx::ShaderHandle fsh = BGFX_INVALID_HANDLE;
  if (!fsName.empty())
  {
    fsh = loadShaderFile(fsName, reload);
  }

  auto shader = makeShader(vsh, fsh);
  resource = shader;

  CLOG(INFO, "PXEngine") << "The shader has been loaded successfully";
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
    case bgfx::RendererType::Direct3D11:
    case bgfx::RendererType::Direct3D12: shaderType = "dx11";  break;
    case bgfx::RendererType::Agc:
    case bgfx::RendererType::Gnm:        shaderType = "pssl";  break;
    case bgfx::RendererType::Metal:      shaderType = "metal"; break;
    case bgfx::RendererType::Nvn:        shaderType = "nvn";   break;
    case bgfx::RendererType::OpenGL:     shaderType = "glsl";  break;
    case bgfx::RendererType::OpenGLES:   shaderType = "essl";  break;
    case bgfx::RendererType::Vulkan:     shaderType = "spirv"; break;

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

  CLOG(INFO, "PXEngine") << "Loaded shader " << filename << " (" << path << ")";
  return handle;
}

px::TexturePtr px::ResourceManager::loadTexture(const std::string &texture, bool reload) {
  EASY_BLOCK("px::ResourceManager::loadTexture")
  Resource &resource = (*this)[texture];
  if (not reload and std::holds_alternative<TexturePtr>(resource)) {
    return std::move(std::get<TexturePtr>(resource));
  }

  std::vector<std::string> texturePath = ResourceManager::parseResourcePath(texture);
  fs::path path = m_rootDir;
  for (const auto& i : texturePath)
    path /= i;

  // check extensions
  auto extensions = {".png", ".jpg", ".jpeg", ".bmp", ".gif", ".tga", ".psd"};
  bool found = false;
  for (auto ext : extensions) {
    path.replace_extension(ext);
    if (not fs::exists(path))
      continue;
    found = true;
  }
  if (not found)
    throw std::runtime_error("Texture not found.");

  auto texturePtr = makeTexture();
  texturePtr->loadFromFile(path);
  resource = texturePtr;

  CLOG(INFO, "PXEngine") << "Loaded texture " << texture << " (" << path << ")";
  return texturePtr;
}

px::ResourceManager::RawType px::ResourceManager::loadRawFile(const std::string &_path, bool reload) {
  EASY_BLOCK("px::ResourceManager::loadRawFile")
  fs::path path = absolutePath(_path);

  Resource &resource = (*this)[_path];
  if (not reload and std::holds_alternative<RawTypeHandler>(resource)) {
    auto &handle = std::get<RawTypeHandler>(resource);
    return RawType(handle.data(), handle.size());
  }

  px::MemoryStream ms;
  ms.loadFromFile(path);

  RawType raw(ms.data(), ms.size());
  resource = std::move(ms);


  CLOG(INFO, "PXEngine") << "Loaded raw " << _path << " (" << path << ")";
  return raw;
}

const px::Localization &px::ResourceManager::loadLocalization(const std::string &localization, bool reload) {
  EASY_BLOCK("px::ResourceManager::loadLocalization")
  Resource &resource = (*this)[localization];
  if (not reload and std::holds_alternative<LocalizationPtr>(resource)) {
    return *std::get<LocalizationPtr>(resource);
  }

  auto _path = parseResourcePath(localization);
  fs::path path = m_rootDir;
  for (const auto &i : _path)
    path /= i;

  auto loc = std::make_unique<Localization>();
  auto *ptr = loc.get();
  loc->setFolder(path);
  // load default lang
  loc->loadLanguage("ru");

  resource = std::move(loc);
  CLOG(INFO, "PXEngine") << "Loaded localization " << localization;
  return *ptr;
}

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
