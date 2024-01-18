#pragma once

namespace px
{
  namespace GraphicsType {
    enum Enum {
      None,
      OpenGL,
      Vulkan,
      CPU,
      DirectX11,
      DirectX12
    };
  }

  struct GraphicsSettings
  {
    int maxFps = 60;
    GraphicsType::Enum graphicsType = GraphicsType::None;
  };

  struct Settings
  {
    GraphicsSettings graphicsSettings;
  };
}
