// Copyright (c) 2023.

//
// Created by pyxiion on 20.11.23.
//

#include <iostream>
#include <easylogging++.h>
#define STB_IMAGE_IMPLEMENTATION
#include "../../../../bgfx.cmake/bimg/3rdparty/stb/stb_image.h"
#include "image.hpp"

namespace px {
  Image::~Image() {
    clear();
  }

  void Image::loadFromFile(const std::string &filename) {
    m_data = stbi_load(filename.c_str(), &m_size.x, &m_size.y, &m_channels, STBI_rgb_alpha);
    if (stbi_failure_reason()) {
      CLOG(ERROR, "PXEngine") << "Failed to load image \"" << filename << "\"";
      throw std::runtime_error(stbi_failure_reason());
    }
  }

  void Image::loadFromStream(std::istream &istream) {
    static stbi_io_callbacks callbacks = {
        &read, &skip, &eof
    };
    stbi_load_from_callbacks(&callbacks, &istream, &m_size.x, &m_size.y, &m_channels, STBI_rgb_alpha);
    if (stbi_failure_reason()) {
      CLOG(ERROR, "PXEngine") << "Failed to load image from stream";
      throw std::runtime_error(stbi_failure_reason());
    }
  }

  void Image::clear() {
    if (m_data) {
      m_channels = 0;
      m_size = {0, 0};
      stbi_image_free((void*)m_data);
      m_data = nullptr;
    }
  }

  Vector2i Image::size() const {
    return m_size;
  }

  int Image::channels() const {
    return m_channels;
  }

  const uint8_t *Image::data() const {
    return m_data;
  }

  auto Image::read(void *user, char *data, int size) -> int {
    return static_cast<int>(((std::istream*)user)->readsome(data, size));
  }

  auto Image::skip(void *user, int n) -> void {
    ((std::istream*)user)->ignore(n);
  }

  auto Image::eof(void *user) -> int {
    return ((std::istream*)user)->eof() ? 1 : 0;
  }
} // px