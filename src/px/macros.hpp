//
// Created by pyxiion on 29.10.23.
//

#ifndef PX_MATH_MACROS_HPP
#define PX_MATH_MACROS_HPP

#define PX_DIVISION_ROUND_UP(x, y) \
  (x) ? (1 + ((x - 1) / y)) : 0

#define U8(str) (str)

#define PX_INT_ENUM(enumValue) (static_cast<int>(enumValue))

#endif //PX_MATH_MACROS_HPP
