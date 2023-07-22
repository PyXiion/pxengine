#pragma once
#include <string>

namespace px
{
  // Тут также указаны значение по-умолчанию
  struct Settings
  {
    int maxFps = 60;

    std::string playerName = "Player";
  };
}
