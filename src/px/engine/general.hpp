#pragma once
#include <string>
#include <string_view>

#define PX_ENGINE_VERSION_MAJOR 0
#define PX_ENGINE_VERSION_MINOR 0
#define PX_ENGINE_VERSION_PATCH 1
#define PX_ENGINE_VERSION      "0.0.1"
#define PX_ENGINE_BUILD         1

namespace px::info
{
  constexpr uint16_t engineMajorVersion  = PX_ENGINE_VERSION_MAJOR;
  constexpr uint16_t engineMinorVersion  = PX_ENGINE_VERSION_MINOR;
  constexpr uint16_t enginePatchVersion  = PX_ENGINE_VERSION_PATCH;
  constexpr uint16_t engineBuild         = PX_ENGINE_BUILD;
  
  inline std::string engineVersion       = PX_ENGINE_VERSION;

  inline std::string engineBuildDateTime = __DATE__ " " __TIME__;

  inline std::string engineWebUrl        = "https://engine.pyxiion.ru/";

  inline std::string glslVersion         = "#version 400";
}
