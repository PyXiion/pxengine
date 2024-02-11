#pragma once
#include <string>
#include <memory>
#include <thread>
#include <atomic>

#include <BS_thread_pool.hpp>
#include <bgfx/bgfx.h>

#include "common/controls.hpp"
#include "events/event_manager.hpp"
#include "system/window.hpp"
#include "ui/debug/info_window.hpp"
#include "world/world.hpp"
#include "common/frame_limiter.hpp"
#include "graphics/renderer.hpp"
#include "graphics/camera.hpp"

#include "common/imgui/imgui.hpp"
#include "px/engine/ui/settings_window.hpp"
#include "settings.hpp"
#include "resources/registries.hpp"

namespace px {
  class Engine : public EventListener {
  public:
    using UpdateCallback = eventpp::CallbackList<void (float)>;
    using DrawCallback   = eventpp::CallbackList<void ()>;

  public:
    Engine();

    ~Engine() override;

    Engine(const Engine &) = delete;

    Engine &operator=(const Engine &) = delete;

    Engine(Engine &&) = delete;

    Engine &operator=(Engine &&) = delete;

    /// @brief Запускает игровой движок.
    void run();

    void loadModule(const std::string &pathToModule);

    void reloadSettings();

    Window &getWindow() const;

    EventManager &getEventManager();

    Controls &getControls();

    Renderer &getRenderer();

    BS::thread_pool &getThreadPool();

    World *getWorld() const;

    Settings &getSettings();

    SettingsWindow &getSettingsWindow();

    Registries &getRegistries() const;

    static Engine &getInstance();

    World &createNewWorld();

    void setCamera(CameraPtr camera);

    CameraPtr getCamera() const;

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

    std::unique_ptr<World> m_world;
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<Registries> m_registries;
    std::unique_ptr<Window> m_window;
    std::unique_ptr<Controls> m_controls;

    std::unique_ptr<std::thread> m_tickLoopThread;

    CameraPtr m_camera;

    BS::thread_pool m_threadPool;

    EventManager m_eventManager;

    std::unique_ptr<ImGuiCtx> m_imgui;
    std::unique_ptr<DebugInfoWindow> m_debugInfoWindow;

    std::unique_ptr<SettingsWindow> m_settingsWindow;
    Settings m_settings;

    int m_maxFps;
    int m_maxTps;
    FrameLimiter m_fpsLimiter{};

    float m_speed;
    float m_deltaTime;
    float m_tickDeltaTime;

    std::atomic<bool> m_tickThreadShouldStop;

    void init();

    void loadCoreResources();

    void registerEventTypes();

    void proxyEvents();

    void loop();

    void draw();

    /// @brief Рисование интерфейса.
    /// @todo Рисовать в отдельном потоке с меньшим FPS (например, в тиках ?).
    void drawImGui();

    void tickThread();

    void tickLoop();
  };
}
