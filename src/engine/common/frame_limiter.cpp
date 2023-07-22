#include "frame_limiter.hpp"
#include <thread>



px::FrameLimiter::FrameLimiter(int maxFps)
  : m_clock()
  , m_frameDelta(std::chrono::microseconds(std::chrono::seconds(1)) / maxFps)
  , m_lastFrame(m_clock.now())
  , m_target(m_lastFrame)
{
}

float px::FrameLimiter::sleep()
{
  std::this_thread::sleep_until(m_target);
  auto now = m_clock.now();

  m_target = now + m_frameDelta;

  using ms = std::chrono::duration<float, std::milli>;
  float deltaTime = std::chrono::duration_cast<ms>(now - m_lastFrame).count();

  m_lastFrame = now;

  return deltaTime;
}
