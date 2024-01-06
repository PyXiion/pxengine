// Copyright (c) 2023.

//
// Created by pyxiion on 20.11.23.
//

#include "texture.hpp"
#include "px/px.hpp"
#include <easylogging++.h>

namespace px {
  void Texture::loadFromFile(const std::string &filename) {
    Image image;
    image.loadFromFile(filename);
    loadFromImage(image);
  }

  void Texture::loadFromImage(const Image &image) {
    EASY_BLOCK("px::Texture::loadFromImage")

    auto size = image.size();
    CLOG(INFO, "PXEngine") << "Creating new texture...";
    CLOG(INFO, "PXEngine") << "\tSize: " << size.x << " " << size.y;
    CLOG(INFO, "PXEngine") << "\tChannels: " << image.channels();

    bgfx::TextureFormat::Enum format;
    if (image.channels() == 3) {
      format = bgfx::TextureFormat::RGB8;
      CLOG(INFO, "PXEngine") << "\tFormat: RGB8";
    } else if (image.channels() == 2) {
      format = bgfx::TextureFormat::RGBA8;
      CLOG(INFO, "PXEngine") << "\tFormat: RGBA8";
    } else {
      format = bgfx::TextureFormat::Unknown;
    }

    auto res =
        bgfx::createTexture2D(
            size.x, size.y,
            false, 1,
            format,
            BGFX_SAMPLER_U_CLAMP
            | BGFX_SAMPLER_V_CLAMP
            | BGFX_SAMPLER_MIN_POINT
            | BGFX_SAMPLER_MAG_POINT,
            bgfx::copy(image.data(), size.x * size.y * image.channels())
        );

    if (!bgfx::isValid(res)) {
      PX_THROW_AND_LOG("PXEngine", std::runtime_error, "Failed to load texture");
    }

    m_handle = res;
  }

  bgfx::TextureHandle Texture::getHandle() const {
    return m_handle;
  }

  namespace resources {
    std::vector<std::string> Traits<Texture>::extensions {
        ".png", ".jpg", ".jpeg", ".tga", ".bmp",
        ".gif", ".hdr", ".pic", ".ppm", ".pgm"
    };

    Resource<Texture> Traits<Texture>::load(std::istream &is) {
      auto image = Image();
      image.loadFromStream(is);

      auto texture = makeResource<Texture>();
      texture->loadFromImage(image);

      return texture;
    }
  }
} // px