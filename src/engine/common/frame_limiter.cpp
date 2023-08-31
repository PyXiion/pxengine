#include "frame_limiter.hpp"
#include <thread>

px::FrameLimiter::FrameLimiter(int maxFps)
  : m_clock()
  , m_lastFrame()
  , m_target()
{
  setMaxFps(maxFps);
}

void px::FrameLimiter::setMaxFps(int maxFps)
{
  if (maxFps != 0.0f)
    m_frameDelta = std::chrono::microseconds(std::chrono::seconds(1)) / maxFps;
  else
    m_frameDelta = std::chrono::seconds(0);

  m_lastFrame = m_clock.now();
  m_target = m_lastFrame + m_frameDelta;
}

float px::FrameLimiter::sleep()
{
  std::this_thread::sleep_until(m_target);

  auto now = m_clock.now();
  m_target = now + m_frameDelta;

  using ms = std::chrono::duration<float, std::milli>;
  float deltaTime = std::chrono::duration_cast<ms>(now - m_lastFrame).count() / 1000.0f;

  m_lastFrame = now;

  return deltaTime;
}
