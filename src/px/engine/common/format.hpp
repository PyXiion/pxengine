#pragma once
#include <string>

namespace px
{
  template<class T>
  inline std::string toString(const T &value)
  {
    return "Unknown type";
  }
}