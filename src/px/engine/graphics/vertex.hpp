// Copyright (c) 2023.

//
// Created by pyxiion on 22.11.23.
//

#ifndef PX_ENGINE_VERTEX_HPP
#define PX_ENGINE_VERTEX_HPP
#include "../math/vector.hpp"

namespace px {
  struct Vertex {
    Vector3 position;
    Vector3 normal;
    Vector2 texCoords;
  };
} // px

#endif //PX_ENGINE_VERTEX_HPP
