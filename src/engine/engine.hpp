#pragma once
#include <string>
#include <memory>
#include <thread>
#include <atomic>

#include "events/event_manager.hpp"
#include "graphics/renderer.hpp"
#include "system/window.hpp"
#include "scripts/engine.hpp"

namespace px
{
  const int ticksPerSecond = 30;

  class Engine
  {
    using UpdateCallback = eventpp::CallbackList<void (float)>;

  public:
    Engine();
    ~Engine();

    /// @brief Запускает игровой движок.
    void run();
    
    /// @brief Не реализовано
    void loadModule(const std::string &path);

    Window &getWindow();
    Renderer &getRenderer();
    ScriptEngine &getScriptEngine();
    EventManager &getEventManager();

    /// @brief Обратный вызов, который вызывается при во время инициализации игрового движка.
    eventpp::CallbackList<void (Engine &)> onInit;
    /// @brief Обратный вызов, который вызывается при завершении работы игрового движка.
    eventpp::CallbackList<void (Engine &)> onExit; 

    /// @brief Обратный вызов, который вызывается во время рисования пользовательского интерфейса.
    eventpp::CallbackList<void ()> onGuiDraw; 

    /// @brief Обратный вызов, который вызывается перед тиком.
    UpdateCallback onPreTick;
    /// @brief Обратный вызов, который вызывается во время тика.
    UpdateCallback onTick;
    /// @brief Обратный вызов, который вызывается после тика.
    UpdateCallback onPostTick;

    /// @brief Обратный вызов, который вызывается перед каждым кадром.
    UpdateCallback onPreUpdate;
    /// @brief Обратный вызов, который вызывается каждый кадр.
    UpdateCallback onUpdate;
    /// @brief Обратный вызов, который вызывается после каждого кадра.
    UpdateCallback onPostUpdate;

  private:
    std::unique_ptr<Window> m_window;
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<ScriptEngine> m_scriptEngine;

    std::unique_ptr<std::thread> m_tickLoopThread;

    EventManager m_eventManager;

    int m_maxFps;
    int m_maxTps;

    float m_speed;
    float m_deltaTime;
    float m_tickDeltaTime;

    std::atomic<bool> m_tickThreadShouldStop;

    void init();
    void loop();
    void draw();

    /// @brief Рисование интерфейса.
    /// @todo Рисовать в отдельном потоке с меньшим FPS (например, в тиках ?).
    void drawImGui();

    void tickThread();
    void tickLoop();
  };
}
