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

namespace px {

  class Texture {
  public:
    void loadFromFile(const std::string &filename);
    void loadFromImage(const Image &image);

    bgfx::TextureHandle getHandle() const;

  private:
    BgfxUniqueTextureHandle m_handle;
  };

  typedef std::shared_ptr<Texture> TexturePtr;
  constexpr auto makeTexture = std::make_shared<Texture>;
} // px

#endif //PX_ENGINE_TEXTURE_HPP
