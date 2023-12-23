// Copyright (c) 2023.

//
// Created by pyxiion on 20.11.23.
//

#ifndef PX_ENGINE_IMAGE_HPP
#define PX_ENGINE_IMAGE_HPP

#include <string>
#include "px/engine/math/vector.hpp"

namespace px {

  class Image {
  public:
    Image() = default;
    ~Image();

    void loadFromFile(const std::string &filename);
    void clear();

    [[nodiscard]] Vector2i size() const;
    [[nodiscard]] int channels() const;
    [[nodiscard]] const uint8_t *data() const;

  private:
    Vector2i m_size{};
    int m_channels{};
    const uint8_t *m_data{};
  };

} // px

#endif //PX_ENGINE_IMAGE_HPP
