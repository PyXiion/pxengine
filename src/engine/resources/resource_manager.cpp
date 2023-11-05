#include "resource_manager.hpp"
#include <filesystem>
#include "../common/utils.hpp"
#include "../engine.hpp"
#include "../utils/bgfx_utils.hpp"

namespace fs = std::filesystem;

px::ResourceManager::ResourceManager(Engine &engine, const std::string &rootDir)
  : m_engine(engine)
  , m_rootDir(rootDir)
{

}

std::string px::ResourceManager::normalizePath(const std::string &_path)
{
  fs::path path = _path;
  if (path.is_absolute())
  {
    return path;
  }

  return fs::absolute(fs::path(m_rootDir) / path);
}

bgfx::ProgramHandle px::ResourceManager::loadProgram(const std::string &vsName, const std::string &fsName) {
  bgfx::ShaderHandle vsh = loadShader(vsName);
  bgfx::ShaderHandle fsh = BGFX_INVALID_HANDLE;
  if (!fsName.empty())
  {
    fsh = loadShader(fsName);
  }

  return bgfx::createProgram(vsh, fsh, true /* destroy shaders when program is destroyed */);
}

bgfx::ShaderHandle px::ResourceManager::loadShader(const std::string &filename) {
  auto shaderPath = fs::path(m_rootDir) / "core/shaders";
  switch (bgfx::getRendererType()) {
    case bgfx::RendererType::Noop:
    case bgfx::RendererType::Direct3D9:  shaderPath /= "dx9"; break;
    case bgfx::RendererType::Direct3D11:
    case bgfx::RendererType::Direct3D12: shaderPath /= "dx11";  break;
    case bgfx::RendererType::Agc:
    case bgfx::RendererType::Gnm:        shaderPath /= "pssl";  break;
    case bgfx::RendererType::Metal:      shaderPath /= "metal"; break;
    case bgfx::RendererType::Nvn:        shaderPath /= "nvn";   break;
    case bgfx::RendererType::OpenGL:     shaderPath /= "glsl";  break;
    case bgfx::RendererType::OpenGLES:   shaderPath /= "essl";  break;
    case bgfx::RendererType::Vulkan:
    case bgfx::RendererType::WebGPU:     shaderPath /= "spirv/"; break;

    case bgfx::RendererType::Count:
      assert(false), "You should not be here!";
      break;
  }

  shaderPath /= filename + ".bin";

  bgfx::ShaderHandle handle = bgfx::createShader(utils::bgfx::stringToMemory(px::io::loadFileToString(shaderPath)));
  bgfx::setName(handle, filename.c_str());
  return handle;
}
