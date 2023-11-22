#pragma once
#include <chrono>

namespace px
{
  class FrameLimiter
  {
  public:
    explicit FrameLimiter(float maxFps = 60);

    void setMaxFps(float maxFps);

    float reset();
    float sleep();

  private:
    using Clock = std::chrono::high_resolution_clock;
    using Duration = std::chrono::microseconds;
    using TimePoint = Clock::time_point;

    Duration m_step;
    TimePoint m_previousFrame;
    TimePoint m_nextFrame;
  };
}
