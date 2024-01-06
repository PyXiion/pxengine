#pragma once
#include <string>

namespace px
{
  template<class T>
  std::string toString([[maybe_unused]] const T &value)
  {
    return "Unknown type";
  }
}