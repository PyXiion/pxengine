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
    void loadFromStream(std::istream &istream);
    void clear();

    [[nodiscard]] Vector2i size() const;
    [[nodiscard]] int channels() const;
    [[nodiscard]] const uint8_t *data() const;

  private:
    Vector2i m_size{};
    int m_channels{};
    const uint8_t *m_data{};

    static int read(void *user, char *data, int size);
    static void skip(void *user, int n);
    static int eof(void *user);
  };

} // px

#endif //PX_ENGINE_IMAGE_HPP
