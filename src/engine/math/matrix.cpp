#include "matrix.hpp"
#include <fmt/format.h>
#include "../common/format.hpp"


template<>
std::string px::toString(const px::Matrix3x3 &mat)
{
  return fmt::format("[{}, {}, {}, {}, {}, {}, {}, {}, {}]",
    mat[0][0], mat[0][1], mat[0][2],
    mat[1][0], mat[1][1], mat[1][2],
    mat[2][0], mat[2][1], mat[2][2]);
}

template<>
std::string px::toString(const px::Matrix4x4 &mat)
{
  return fmt::format("[{}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}, {}]",
    mat[0][0], mat[0][1], mat[0][2], mat[0][3],
    mat[1][0], mat[1][1], mat[1][2], mat[1][3],
    mat[2][0], mat[2][1], mat[2][2], mat[2][3],
    mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
}
