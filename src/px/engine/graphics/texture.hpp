// Copyright (c) 2023.

//
// Created by pyxiion on 20.11.23.
//

#ifndef PX_ENGINE_TEXTURE_HPP
#define PX_ENGINE_TEXTURE_HPP
#include <memory>
#include <bgfx/bgfx.h>
#include "../resources/bgfx_handle.hpp"
#include "image.hpp"
#include "px/memory/ref_counting.hpp"

namespace px {

  class Texture : public RefCounting {
  public:
    void loadFromFile(const std::string &filename);
    void loadFromImage(const Image &image);

    [[nodiscard]] bgfx::TextureHandle getHandle() const;

  private:
    BgfxUniqueTextureHandle m_handle;
  };
} // px

#endif //PX_ENGINE_TEXTURE_HPP
