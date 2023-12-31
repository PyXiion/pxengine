//
// Created by pyxiion on 22.11.23.
//

#include "shader.hpp"
#include <filesystem>
#include <ranges>
#include <yaml-cpp/yaml.h>
#include <easylogging++.h>
#include "px/engine/resources/resource_manager.hpp"
#include "px/engine/utils/bgfx_utils.hpp"
#include "model.hpp"


namespace fs = std::filesystem;

namespace px {
  Shader::Shader(bgfx::ShaderHandle vs, bgfx::ShaderHandle fs) {
    m_program = bgfx::createProgram(vs, fs, false);
  }

  bgfx::ProgramHandle Shader::get() const {
    return m_program;
  }

  std::string
  resources::Traits<BgfxUniqueShaderHandle>::getPath(const std::string &root, const std::string &resourceId) {
    fs::path path = root;
    auto parsed = parseId(resourceId);
    auto folders = parsed.size() - 1;

    for (auto &i : parsed | std::views::take(folders)) {
      path /= i;
    }

    static std::unordered_map<bgfx::RendererType::Enum, std::string> types {
        {bgfx::RendererType::Vulkan, "spirv"},
        {bgfx::RendererType::OpenGL, "glsl"},
    };
    path /= types[bgfx::getRendererType()]; // type
    path /= parsed[folders]; // add filename

    path.replace_extension(".bin");
    return path;
  }

  Resource<BgfxUniqueShaderHandle> resources::Traits<BgfxUniqueShaderHandle>::load(std::istream &is) {
    CLOG(INFO, "PXEngine") << "Loading shader " << getCurrentResourceId();
    std::string data {std::istreambuf_iterator<char>(is),
                      std::istreambuf_iterator<char>()};
    auto memory = px::utils::bgfx::stringToMemory(data);
    auto shader = bgfx::createShader(memory);

    return makeResource<BgfxUniqueShaderHandle>(shader);
  }

  std::vector<std::string> resources::Traits<Shader>::extensions {
    ".yml", ".yaml"
  };

  Resource<Shader> resources::Traits<Shader>::load(ResourceManager &manager, std::istream &is) {
    CLOG(INFO, "PXEngine") << "Loading shader program " << getCurrentResourceId();
    YAML::Node config = YAML::Load(is);

    auto vsShaderId = config["vertex"].as<std::string>();
    auto fsShaderId = config["fragment"].as<std::string>();

    auto vertex = manager.get<BgfxUniqueShaderHandle>(vsShaderId);
    auto fragment = manager.get<BgfxUniqueShaderHandle>(fsShaderId);

    auto shader = makeResource<Shader>(vertex->get(), fragment->get());
    return shader;
  }
} // px