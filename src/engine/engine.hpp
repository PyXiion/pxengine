#pragma once
#include <string>
#include <memory>
#include <thread>
#include <atomic>

#include "graphics/renderer.hpp"
#include "system/window.hpp"
#include "scripts/engine.hpp"

namespace px
{
  const int ticksPerSecond = 30;

  class Engine
  {
  public:
    Engine();
    ~Engine();

    void run();

    void loadModule(const std::string &path);

    Window &getWindow();
    Renderer &getRenderer();
    ScriptEngine &getScriptEngine();

    eventpp::CallbackList<void (Engine &)> onInit;

    eventpp::CallbackList<void (float)> onTick;
    eventpp::CallbackList<void (float)> onUpdate;

  private:
    std::unique_ptr<Window> m_window;
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<ScriptEngine> m_scriptEngine;

    std::unique_ptr<std::jthread> m_tickLoopThread;

    const int m_maxFps;
    const int m_maxTps;

    float m_speed;
    float m_deltaTime;
    float m_tickDeltaTime;

    std::atomic<bool> m_tickThreadShouldStop;

    void init();

    void loop();
    void draw();

    void tickThread();
    void tickLoop();

  };
}
