// Copyright (c) 2024.

//
// Created by pyxiion on 03.02.24.
//
#include "registries.hpp"

#include <ranges>

px::Registries::Registries() {
  using namespace BuiltinRegistryKeys;

  REGISTRIES[TextsRegistry]         = &TEXTS;
  REGISTRIES[TexturesRegistry]      = &TEXTS;
  REGISTRIES[ShadersRegistry]       = &SHADERS;
  REGISTRIES[ModelsRegistry]        = &MODELS;
  REGISTRIES[LocalizationsRegistry] = &LOCALIZATIONS;
  REGISTRIES[VertexBuffersRegistry] = &VERTEX_BUFFERS;
  REGISTRIES[IndexBuffersRegistry]  = &INDEX_BUFFERS;
  REGISTRIES[FontsRegistry]         = &FONTS;
}

void px::Registries::clear() {
  for (auto &registry : REGISTRIES | std::views::values) {
    registry->clear();
  }
}
