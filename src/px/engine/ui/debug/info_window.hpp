#pragma once
#include <vector>
#include <atomic>
#include "../../world/game_object.hpp"
#include "../../events/event_listener.hpp"

namespace px
{
  class Engine;
  class Localization;
  class World;

  class DebugInfoWindow : private EventListener {
  public:
    explicit DebugInfoWindow(Engine &engine);

  private:
    const int kGraphPointCount = 50;

    Engine &m_engine;
    Ref<Localization> m_localization;

    std::atomic<float> m_frameDeltaTime;
    std::atomic<float> m_tickDeltaTime;
    std::atomic<float> m_secondTimer;

    std::vector<float> m_frames;
    std::vector<float> m_ticks;

    GameObject *m_worldSelectedObject;

    void onUpdate(float dt);
    void onTick(float dt);

    void onGuiDraw();

    void drawGraph(const char *label, std::vector<float> &graph, float currentValue, bool update) const;
  };
} // namespace px
