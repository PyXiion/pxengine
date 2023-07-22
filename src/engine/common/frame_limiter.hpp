#pragma once
#include <chrono>

namespace px
{
  class FrameLimiter
  {
  public:
    FrameLimiter(int maxFps = 60);

    float sleep();

  private:
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    Clock m_clock;
    std::chrono::microseconds m_frameDelta;

    TimePoint m_lastFrame;
    TimePoint m_target;
  };
}
