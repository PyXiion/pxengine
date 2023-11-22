#include "vector.hpp"
#include <fmt/format.h>
#include "../common/format.hpp"


template<>
std::string px::toString(const px::Vector2 &vec)
{
  return fmt::format("[{}, {}]", vec.x, vec.y);
}

template<>
std::string px::toString(const px::Vector3 &vec)
{
  return fmt::format("[{}, {}, {}]", vec.x, vec.y, vec.z);
}
