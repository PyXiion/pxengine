#include "engine.hpp"
#include <utility>
#include <filesystem>

#include <easy/profiler.h>
#include "easylogging++.h"
#include "common/frame_limiter.hpp"
#include "px/engine/events/common/mouse_event.hpp"
#include "px/engine/events/common/key_event.hpp"
#include "px/engine/scripts/binding.hpp"
#include "px/engine/resources/localization.hpp"

static std::once_flag createLoggerFlag;
static void createLogger() {
  auto logger = el::Loggers::getLogger("PXEngine");

  auto conf = *logger->configurations();
  conf.set(el::Level::Global,
           el::ConfigurationType::Format,
           "%user [%datetime] [%level] \t%logger : %msg");
  logger->configure(conf);
}

px::Engine::Engine()
  : m_maxFps(60)
  , m_maxTps(10)
  , m_deltaTime(1.0f / float(m_maxFps))
  , m_tickDeltaTime(1.0f / float(m_maxTps))
  , m_tickThreadShouldStop(false)
  , m_debugInfoWindow(*this)
  , m_settingsWindow(*this)
{
  std::call_once(createLoggerFlag, createLogger);
  instance = this;
  CLOG(INFO, "PXEngine") << "Created an engine instance";
  CLOG(INFO, "PXEngine") << "The orking directory is " << std::filesystem::current_path();

  EASY_PROFILER_ENABLE
}

px::Engine::~Engine()
{
  // Destruct the world and its objects before other things
  m_world.reset();

  profiler::dumpBlocksToFile("./profile");
  CLOG(INFO, "PXEngine") << "The profile is dumped";
}

void px::Engine::run()
{
  init();
  CLOG(INFO, "PXEngine") << "The engine is fully initialised";

  m_fpsLimiter.reset();
  while (!m_window->isShouldClose())
  {
    loop();
//    m_deltaTime = m_fpsLimiter.sleep();
  }
  m_window->close();

  // Завершение работы

  m_tickThreadShouldStop = true;
  m_tickLoopThread->join();

  onExit(*this);
}

void px::Engine::loadModule(const std::string &pathToModule) {
  ModulePtr ptr = makeModule(*this, pathToModule);

  ptr->load();

  m_modules.push_back(std::move(ptr));
}

void px::Engine::reloadSettings() {
  // graphics
  m_fpsLimiter.setMaxFps(m_settings.graphicsSettings.maxFps);
  CLOG(INFO, "PXEngine") << "The settings have been reloaded";
}

void px::Engine::init()
{
  EASY_BLOCK("px::Engine::init", profiler::colors::Orange)

  registerEventTypes();
  CLOG(INFO, "PXEngine") << "";

  m_resourceManager = std::make_unique<ResourceManager>("./data");

  // set default language
  auto lang = m_resourceManager->get<Localization>("core.lang.ru");
  m_resourceManager->set("core.lang", lang);

  m_window = std::make_unique<Window>("PXE", 1280, 720);

  m_controls = std::make_unique<Controls>(*this, *m_window);

  m_renderer = std::make_unique<Renderer>(*m_window);
  m_renderer->setDebugEnabled(false);

  listen(m_window->onKeyPressed, [this](auto keycode, auto mods) {
    if (m_renderer && keycode == KeyCode_F1) {
      m_renderer->setDebugEnabled(!m_renderer->isDebugEnabled());
    }
  });

  m_imgui = std::make_unique<ImGuiCtx>(*this, *m_window);
  m_imgui->Create();

  m_tickLoopThread = std::make_unique<std::thread>([this] { tickThread(); });

  bind::bindEngine(m_scriptEngine);

  proxyEvents();

  // Core module
  loadModule("./data/core");

  // Инициализация всего остального
  EASY_BLOCK("Init event", profiler::colors::LightBlue)
  onInit(*this);
}

void px::Engine::registerEventTypes() {
  m_eventManager.registerEventClass<MouseEvent>();

  m_eventManager.registerEventClass<KeyEvent>();
  m_eventManager.registerEventClass<KeyReleasedEvent>();
  m_eventManager.registerEventClass<KeyPressedEvent>();
}

void px::Engine::proxyEvents() {
  if (m_window) {
    m_window->onMouseMoved.append([this] (float x, float y) {
      m_eventManager.emplaceEvent<MouseEvent>(x, y);
    });

    m_window->onKey.append([this] (KeyCode key, bool pressed, KeyModifiers::Enum mods) {
      m_eventManager.emplaceEvent<KeyEvent>(key, pressed);

      if (pressed)
        m_eventManager.emplaceEvent<KeyPressedEvent>(key);
      else
        m_eventManager.emplaceEvent<KeyReleasedEvent>(key);
    });
  }
}

void px::Engine::loop()
{
  EASY_BLOCK("px::Engine::loop")

  EASY_BLOCK("Polling events")
  m_window->pollEvents();
  EASY_END_BLOCK

  EASY_BLOCK("Updating other things");
  onPreUpdate(m_deltaTime);
  onUpdate(m_deltaTime);
  onPostUpdate(m_deltaTime);
  EASY_END_BLOCK

  if (m_camera) {
    Vector3 offset{};
    offset += m_camera->getForward() * m_controls->getAxis(ControlAxis::Vertical);
    offset += -m_camera->getRight() * m_controls->getAxis(ControlAxis::Horizontal);
    m_camera->move(offset * m_deltaTime);
  }

  draw();
}

void px::Engine::draw()
{
  if (!m_renderer)
    return;
  EASY_BLOCK("px::Engine::draw")

  drawImGui();

  EASY_BLOCK("Graphics");

  if (m_camera) {
    m_camera->apply();
  }
  m_renderer->beginFrame();
  onDraw();
  m_renderer->renderFrame();
}

void px::Engine::drawImGui()
{
  EASY_BLOCK("px::Engine::drawImGui")
  m_imgui->BeginFrame(255);

  onGuiDraw();

  m_imgui->EndFrame();
}

void px::Engine::tickThread()
{
  EASY_THREAD_SCOPE("Tick thread");
  FrameLimiter tickLimiter(m_maxTps);

  while (!m_tickThreadShouldStop)
  {
    tickLoop();
    m_tickDeltaTime = tickLimiter.sleep();
  }
}

void px::Engine::tickLoop()
{
  EASY_BLOCK("px::Engine::tickLoop");

  m_eventManager.process();

  onPreTick(m_tickDeltaTime);
  onTick(m_tickDeltaTime);
  onPostTick(m_tickDeltaTime);
}

px::Window &px::Engine::getWindow() const
{
  return *m_window;
}

px::ResourceManager &px::Engine::getResourceManager() const
{
  return *m_resourceManager;
}

px::EventManager &px::Engine::getEventManager()
{
  return m_eventManager;
}

px::Controls &px::Engine::getControls()
{
  return *m_controls;
}

px::Renderer &px::Engine::getRenderer() {
  return *m_renderer;
}

BS::thread_pool &px::Engine::getThreadPool()
{
  return m_threadPool;
}

px::World *px::Engine::getWorld() const
{
  return m_world.get();
}

px::Settings &px::Engine::getSettings() {
  return m_settings;
}

px::SettingsWindow &px::Engine::getSettingsWindow() {
  return m_settingsWindow;
}

px::script::AngelScript &px::Engine::getScriptEngine() {
  return m_scriptEngine;
}

px::World &px::Engine::createNewWorld()
{
  EASY_BLOCK("px::Engine::createNewWorld");
  m_world = std::make_unique<World>(*this);
  return *m_world;
}

void px::Engine::setCamera(px::CameraPtr camera) {
  m_camera = std::move(camera);
}

px::Engine *px::Engine::instance = nullptr;

px::Engine &px::Engine::getInstance() {
  if (instance == nullptr) {
    throw std::runtime_error("Engine instance is null.");
  }
  return *instance;
}

px::CameraPtr px::Engine::getCamera() const {
  return m_camera;
}
