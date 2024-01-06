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
    if (not m_data) {
      CLOG(ERROR, "PXEngine") << "Failed to load image \"" << filename << "\"";
      throw std::runtime_error(stbi_failure_reason());
    }
  }

  void Image::loadFromStream(std::istream &istream) {
    static stbi_io_callbacks callbacks = {
        &read, &skip, &eof
    };
    m_data = stbi_load_from_callbacks(&callbacks, &istream, &m_size.x, &m_size.y, &m_channels, STBI_rgb_alpha);
    if (not m_data) {
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

  int Image::read(void *user, char *data, int size) {
    auto stream = (std::istream*)user;

    stream->read(data, size);
    return size;
  }

  void Image::skip(void *user, int n) {
    auto stream = (std::istream*)user;
    stream->ignore(n);
  }

  int Image::eof(void *user) {
    auto stream = (std::istream*)user;
    return stream->eof() ? 1 : 0;
  }
} // px