#pragma once
#include <vector>
#include <atomic>

namespace px
{
  class Engine;

  class DebugInfoWindow
  {
  public:
    DebugInfoWindow(Engine &engine);

  private:
    const int kGraphPointCount = 40;

    Engine &m_engine;

    std::atomic<float> m_frameDeltaTime;
    std::atomic<float> m_tickDeltaTime;
    std::atomic<float> m_secondTimer;

    std::vector<float> m_frames;
    std::vector<float> m_ticks;

    void onUpdate(float dt);
    void onTick(float dt);

    void onGuiDraw();

    void drawGraph(const char *label, std::vector<float> &graph, float currentValue, bool update);
  };
} // namespace px
