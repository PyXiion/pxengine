// Copyright (c) 2024.

//
// Created by pyxiion on 03.02.24.
//

#ifndef PX_ENGINE_RESOURCES_REGISTRY_HPP
#define PX_ENGINE_RESOURCES_REGISTRY_HPP
#include <easylogging++.h>
#include <unordered_map>
#include <utility>
#include <fmt/format.h>

#include "px/px.hpp"

namespace px {
  class IRegistry {
  public:
    [[nodiscard]] virtual const std::string &getRegistryKey() const = 0;

    virtual void clear() = 0;

  protected:
    virtual ~IRegistry() = default;
  };

  template<class T>
  class Registry final : public IRegistry {
  public:
    explicit Registry(std::string registryKey);

    ~Registry() override = default;

    T &get(const std::string &resourceId) {
      try {
        return m_cached.at(resourceId);
      } catch (const std::out_of_range &e) {
        PX_THROW_AND_LOG("PXEngine", std::out_of_range, "Failed to get resource '{}' at '{}' registry", resourceId,
                         getRegistryKey());
      }
    }

    const T &get(const std::string &resourceId) const {
      try {
        return m_cached.at(resourceId);
      } catch (const std::out_of_range &) {
        PX_THROW_AND_LOG("PXEngine", std::out_of_range, "Failed to get resource '{}' at '{}' registry", resourceId,
                         getRegistryKey());
      }
    }

    void set(const std::string &resourceId, T resource) {
      CLOG(INFO, "PXEngine") << "Updated resource " << resourceId;
      m_cached[resourceId] = std::move(resource);
    }

    void copy(const std::string &toResourceId, const std::string &fromResourceId) {
      set(toResourceId, get(fromResourceId));
    }

    [[nodiscard]] const std::string &getRegistryKey() const override {
      return m_registryKey;
    }

    void clear() override;

    auto begin() { return m_cached.begin(); }

    auto end() { return m_cached.end(); }

  private:
    std::unordered_map<std::string, T> m_cached;
    const std::string m_registryKey;
  };

  template<class T>
  Registry<T>::Registry(std::string registryKey)
    : m_registryKey(std::move(registryKey)) {}

  template<class T>
  void Registry<T>::clear() {
    m_cached.clear();
  }
} // px

#endif //PX_ENGINE_RESOURCES_REGISTRY_HPP
