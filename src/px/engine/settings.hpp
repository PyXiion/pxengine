#pragma once
#include <string>

namespace px
{
  struct GraphicsSettings
  {
    int maxFps = 60;
  };

  struct Settings
  {
    GraphicsSettings graphicsSettings;
  };
}
