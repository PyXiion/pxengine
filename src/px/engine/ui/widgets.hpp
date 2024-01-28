// Copyright (c) 2023.

//
// Created by pyxiion on 20.11.23.
//

#ifndef PX_ENGINE_WIDGETS_HPP
#define PX_ENGINE_WIDGETS_HPP
#include "../graphics/texture.hpp"

namespace ImGui::px {
  using namespace ::px;

  void Texture(const TexturePtr &ptr);
  void TexturePtrInput(const char *id, TexturePtr &ptr);
}

#endif //PX_ENGINE_WIDGETS_HPP
