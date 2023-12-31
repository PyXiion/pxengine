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
#include "px/engine/resources/resource_traits.hpp"

namespace px {

  class Texture {
  public:
    void loadFromFile(const std::string &filename);
    void loadFromImage(const Image &image);

    [[nodiscard]] bgfx::TextureHandle getHandle() const;

  private:
    BgfxUniqueTextureHandle m_handle;
  };

  typedef std::shared_ptr<Texture> TexturePtr;
  constexpr auto makeTexture = std::make_shared<Texture>;


  namespace resources {
    template<>
    struct Traits<Texture> {
      static std::vector<std::string> extensions;

      static Resource<Texture> load(std::istream &ifs);;
    };
  }
} // px

#endif //PX_ENGINE_TEXTURE_HPP
