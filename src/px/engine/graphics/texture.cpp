// Copyright (c) 2023.

//
// Created by pyxiion on 20.11.23.
//

#include "texture.hpp"

namespace px {
  void Texture::loadFromFile(const std::string &filename) {
    Image image;
    image.loadFromFile(filename);
    loadFromImage(image);
  }

  void Texture::loadFromImage(const Image &image) {
    EASY_BLOCK("px::Texture::loadFromImage")
    auto size = image.size();
    auto res =
        bgfx::createTexture2D(
            size.x, size.y,
            false, 1,
            bgfx::TextureFormat::RGBA8,
            BGFX_SAMPLER_U_CLAMP
            | BGFX_SAMPLER_V_CLAMP
            | BGFX_SAMPLER_MIN_POINT
            | BGFX_SAMPLER_MAG_POINT,
            bgfx::copy(image.data(), size.x * size.y * image.channels())
        );

    if (!bgfx::isValid(res)) {
      throw std::runtime_error("Invalid texture.");
    }

    m_handle = res;
  }

  bgfx::TextureHandle Texture::getHandle() const {
    return m_handle;
  }
} // px