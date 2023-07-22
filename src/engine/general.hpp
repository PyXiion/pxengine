#pragma once
#include <string_view>

namespace px::info
{
  const int engineMajorVersion = 0;
  const int engineMinorVersion = 0;
  const int enginePatchVersion = 1;

  constexpr std::string_view engineBuildDateTime = __DATE__ " " __TIME__;

  constexpr std::string_view engineWebUrl = "https://engine.pyxiion.ru/";
}
