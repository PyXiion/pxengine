#pragma once
#include <string>
#include <memory>
#include <thread>
#include <atomic>

#include <BS_thread_pool.hpp>
#include <bgfx/bgfx.h>

#include "common/controls.hpp"
#include "events/event_manager.hpp"
#include "resources/resource_manager.hpp"
#include "system/window.hpp"
#include "ui/debug/info_window.hpp"
#include "world/world.hpp"
#include "common/frame_limiter.hpp"
#include "graphics/renderer.hpp"
#include "graphics/camera.hpp"

namespace px
{
  const int ticksPerSecond = 30;

  class Engine
  {
  public:
    using UpdateCallback = eventpp::CallbackList<void (float)>;
    using DrawCallback = eventpp::CallbackList<void ()>;

  public:
    Engine();
    ~Engine();

    /// @brief Запускает игровой движок.
    void run();
    
    /// @brief Не реализовано
    void loadModule(const std::string &path);

    Window &getWindow() const;
    ResourceManager &getResourceManager() const;
    EventManager &getEventManager();
    Controls &getControls();
    Renderer &getRenderer();
    BS::thread_pool &getThreadPool();

    static Engine &getInstance();

    World *getWorld() const;

    px::World &createNewWorld();

    void setCamera(CameraPtr camera);

    /// @brief Обратный вызов, который вызывается при во время инициализации игрового движка.
    eventpp::CallbackList<void (Engine &)> onInit;
    /// @brief Обратный вызов, который вызывается при завершении работы игрового движка.
    eventpp::CallbackList<void (Engine &)> onExit; 

    /// @brief Обратный вызов, который вызывается во время рисования пользовательского интерфейса.
    DrawCallback onDraw; 
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
    static Engine *instance;

    std::unique_ptr<Window> m_window;
    std::unique_ptr<World> m_world;
    std::unique_ptr<ResourceManager> m_resourceManager;
    std::unique_ptr<Renderer> m_renderer;

    std::unique_ptr<std::thread> m_tickLoopThread;

    CameraPtr m_camera;

    BS::thread_pool m_threadPool;
    Controls m_controls;

    EventManager m_eventManager;
    DebugInfoWindow m_debugInfoWindow;

    int m_maxFps;
    int m_maxTps;
    FrameLimiter m_fpsLimiter{};

    const bgfx::ViewId kClearView = 0;
    bool m_showBgfxStats;

    float m_speed;
    float m_deltaTime;
    float m_tickDeltaTime;

    std::atomic<bool> m_tickThreadShouldStop;

    void init();
    void loop();
    void draw() const;

    /// @brief Рисование интерфейса.
    /// @todo Рисовать в отдельном потоке с меньшим FPS (например, в тиках ?).
    void drawImGui();

    void tickThread();
    void tickLoop();
  };
}
