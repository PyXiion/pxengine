#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <any>
#include <fmt/format.h>
#include <easylogging++.h>
#include "resource_traits.hpp"

namespace px {
  class Engine;

  template<class T>
  class ResourceTraits;

  class ResourceManager
  {
  public:
    using Resource = std::any;

  public:
    explicit ResourceManager(std::string rootDir);
    ~ResourceManager();

    ResourceManager(const ResourceManager &) = delete;
    ResourceManager &operator=(const ResourceManager &) = delete;

    ResourceManager(ResourceManager &&) = delete;
    ResourceManager &operator=(ResourceManager &&) = delete;

    Resource &operator[](const std::string &key);

    template<class T>
    auto get(const std::string &resourceId, bool loadIfNotExists = true) -> px::Resource<T> {
      if (not m_cached.contains(resourceId) or not m_cached[resourceId].has_value()) {
        if (loadIfNotExists)
          return load<T>(resourceId);
        else {
          CLOG(ERROR, "PXEngine") << "Resource \"" << resourceId << "\" not found";
          throw std::runtime_error(fmt::format("Resource \"{}\" not found", resourceId));
        }
      }


      auto &any = m_cached[resourceId];

      if (any.type() != typeid(px::Resource<T>)) {
        CLOG(ERROR, "PXEngine") << "The requested resource type does not match the requested type. "
                                << any.type().name() << " vs " << typeid(T).name();

        throw std::runtime_error("The requested resource type does not match the requested type");

      } else {
        return std::any_cast<px::Resource<T>>(any);
      }
    }

    template<class T>
    void set(const std::string &resourceId, px::Resource<T> resource) {
      m_cached[resourceId] = std::forward<px::Resource<T>>(resource);
    }

    template<class T>
    auto load(const std::string &resourceId) -> px::Resource<T> {
      using Traits = resources::Traits<T>;

      resources::setCurrentResourceId(resourceId);
      std::string path = resources::getPath<Traits>(m_rootDir, resourceId);

      std::ifstream ifs(path);
      if (not ifs.is_open()) {
        CLOG(ERROR, "PXEngine") << "Failed to open \"" << path + "\"";
        throw std::runtime_error("Failed to open \"" + path + "\"");
      }

      px::Resource<T> ptr = resources::load<Traits>(*this, ifs);
      set(resourceId, ptr);
      return ptr;
    }

  private:
    std::string m_rootDir;

    std::unordered_map<std::string, Resource> m_cached;
  };
} // namespace px
