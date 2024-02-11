// Copyright (c) 2024.

//
// Created by pyxiion on 03.02.24.
//

#ifndef PX_ENGINE_RESOURCES_REGISTRIES_HPP
#define PX_ENGINE_RESOURCES_REGISTRIES_HPP

#include "localization.hpp"
#include "registry.hpp"
#include "px/engine/graphics/model.hpp"
#include "px/engine/graphics/shader.hpp"
#include "px/engine/graphics/texture.hpp"
#include "px/memory/ref.hpp"

namespace px {
  using RegistryKey = std::string;

  /* enum */
  namespace BuiltinRegistryKeys {
    inline const RegistryKey TextsRegistry         = "core.texts";
    inline const RegistryKey TexturesRegistry      = "core.textures";
    inline const RegistryKey ShadersRegistry       = "core.shaders";
    inline const RegistryKey ModelsRegistry        = "core.models";
    inline const RegistryKey LocalizationsRegistry = "core.localizations";
    inline const RegistryKey VertexBuffersRegistry = "core.vb";
    inline const RegistryKey IndexBuffersRegistry  = "core.ib";
    inline const RegistryKey FontsRegistry         = "core.fonts";
  }

  struct Registries {
    using Binary = std::vector<char>;

    explicit Registries();

    Registry<std::string> TEXTS{BuiltinRegistryKeys::TextsRegistry};
    Registry<Ref<Texture>> TEXTURES{BuiltinRegistryKeys::TexturesRegistry};
    Registry<Ref<Shader>> SHADERS{BuiltinRegistryKeys::ShadersRegistry};
    Registry<Ref<Model>> MODELS{BuiltinRegistryKeys::ModelsRegistry};
    Registry<Ref<Localization>> LOCALIZATIONS{BuiltinRegistryKeys::LocalizationsRegistry};
    Registry<BgfxUniqueVertexBufferHandle> VERTEX_BUFFERS{BuiltinRegistryKeys::VertexBuffersRegistry};
    Registry<BgfxUniqueIndexBufferHandle> INDEX_BUFFERS{BuiltinRegistryKeys::IndexBuffersRegistry};
    Registry<std::vector<char>> FONTS{BuiltinRegistryKeys::FontsRegistry};

    std::unordered_map<std::string, IRegistry *> REGISTRIES;

    template<class T>
    Registry<T> *getRegistry(const std::string &key) {
      auto *registry = REGISTRIES.at(key);
      return dynamic_cast<Registry<T> *>(registry);
    }

  private:
    friend class Engine;

    void clear();
  };
}

#endif //PX_ENGINE_RESOURCES_REGISTRIES_HPP
