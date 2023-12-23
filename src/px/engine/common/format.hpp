#pragma once
#include <string>

namespace px
{
  template<class T>
  inline std::string toString([[maybe_unused]] const T &value)
  {
    return "Unknown type";
  }
}