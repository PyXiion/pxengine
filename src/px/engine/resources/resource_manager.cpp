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

px::ResourceManager::ResourceManager(std::string rootDir)
  : m_rootDir(std::move(rootDir))
  , m_cached()
{
}

px::ResourceManager::~ResourceManager() = default;

px::ResourceManager::Resource &px::ResourceManager::operator[](const std::string &key) {
  return m_cached[key];
}