#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace px
{
  typedef glm::vec2 Vector2;
  typedef glm::vec3 Vector3;

  constexpr Vector2 Vector2Up = Vector2(0, 1);
  constexpr Vector3 Vector3Up = Vector3(0, 1, 0);
}

