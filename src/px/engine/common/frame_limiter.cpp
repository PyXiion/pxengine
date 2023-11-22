#include "frame_limiter.hpp"
#include <thread>

using namespace std::chrono_literals;

px::FrameLimiter::FrameLimiter(float maxFps) {
  setMaxFps(maxFps);
  reset();
}

void px::FrameLimiter::setMaxFps(float maxFps) {
  if (maxFps >= 0.0f)
    m_step = std::chrono::duration_cast<decltype(m_step)>(1s / maxFps);
  else
    m_step = 0s;
}

float px::FrameLimiter::reset() {

  auto now = Clock::now();
  auto dif = now - m_previousFrame;

  m_previousFrame = now;
  m_nextFrame = now + m_step;

  float delta = static_cast<float>(dif.count()) / static_cast<float>(decltype(dif)::period::den);
  return delta;
}

float px::FrameLimiter::sleep() {
  float delta = reset();
  std::this_thread::sleep_until(m_nextFrame);
  return delta;
}
