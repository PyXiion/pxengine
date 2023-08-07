#pragma once
#include <string>
#include <string_view>

#define PX_ENGINE_VERSION "0.0.1"

namespace px::info
{
  constexpr int engineMajorVersion = 0;
  constexpr int engineMinorVersion = 0;
  constexpr int enginePatchVersion = 1;
  
  constexpr std::string_view engineVersion = PX_ENGINE_VERSION;

  constexpr std::string_view engineBuildDateTime = __DATE__ " " __TIME__;

  constexpr std::string_view engineWebUrl = "https://engine.pyxiion.ru/";

  constexpr std::string_view glslVersion = "#version 400";
}
