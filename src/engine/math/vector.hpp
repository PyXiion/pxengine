#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace px
{
  typedef glm::vec2 Vector2;
  typedef glm::vec3 Vector3;

  typedef glm::vec<2, int, glm::defaultp> Vector2i;
  typedef glm::vec<3, int, glm::defaultp> Vector3i;

  typedef glm::vec<2, unsigned int, glm::defaultp> Vector2u;
  typedef glm::vec<3, unsigned int, glm::defaultp> Vector3u;

  constexpr Vector2 Vector2Up = Vector2(0, 1);
  constexpr Vector3 Vector3Up = Vector3(0, 1, 0);

  constexpr Vector2i Vector2iUp = Vector2(0, 1);
  constexpr Vector3i Vector3iUp = Vector3(0, 1, 0);

  constexpr Vector2u Vector2uUp = Vector2(0, 1);
  constexpr Vector3u Vector3uUp = Vector3(0, 1, 0);
}

