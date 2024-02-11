#include "engine.hpp"
#include <utility>
#include <filesystem>

#include <easy/profiler.h>
#include <yaml-cpp/node/node.h>
#include <yaml-cpp/node/parse.h>

#include "easylogging++.h"
#include "common/frame_limiter.hpp"
#include "common/utils.hpp"
#include "px/px.hpp"
#include "px/engine/events/common/mouse_event.hpp"
#include "px/engine/events/common/key_event.hpp"
#include "px/engine/resources/localization.hpp"
#include "utils/bgfx_utils.hpp"

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
, m_tickThreadShouldStop(false) {
  std::call_once(createLoggerFlag, createLogger);
  instance = this;
  CLOG(INFO, "PXEngine") << "Created an engine instance at " << this;
  CLOG(INFO, "PXEngine") << "The orking directory is " << std::filesystem::current_path();

  EASY_PROFILER_ENABLE
}

px::Engine::~Engine() {
  profiler::dumpBlocksToFile("./profile");
  CLOG(INFO, "PXEngine") << "The profile is dumped";
}

void px::Engine::run() {
  init();
  CLOG(INFO, "PXEngine") << "The engine is fully initialised";

  m_fpsLimiter.reset();
  while (!m_window->isShouldClose()) {
    loop();
    //    m_deltaTime = m_fpsLimiter.sleep();
  }
  m_window->close();

  // Завершение работы

  m_tickThreadShouldStop = true;
  m_tickLoopThread->join();

  onExit(*this);

  // Destruct the world and its objects before other things
  m_world.reset();
}

void px::Engine::loadModule(const std::string &pathToModule) {}

void px::Engine::reloadSettings() {
  // graphics
  m_fpsLimiter.setMaxFps(m_settings.graphicsSettings.maxFps);
  CLOG(INFO, "PXEngine") << "The settings have been reloaded";
}

void px::Engine::init() {
  EASY_BLOCK("px::Engine::init", profiler::colors::Orange)

  registerEventTypes();

  m_registries = std::make_unique<Registries>();

  m_window = std::make_unique<Window>("PXE", 1280, 720);

  m_controls = std::make_unique<Controls>(*this, *m_window);

  m_renderer = std::make_unique<Renderer>(*m_window);
  m_renderer->setDebugEnabled(false);

  loadCoreResources();

  PX_IF_DEBUG {
    // switch render debug info on F1
    listen(m_window->onKeyPressed, [this](auto keycode, auto mods) {
      if (m_renderer && keycode == KeyCode_F1) {
        m_renderer->setDebugEnabled(!m_renderer->isDebugEnabled());
      }
    });
  }

  // Create GUI
  m_imgui = std::make_unique<ImGuiCtx>(*this, *m_window);
  m_imgui->Create();

  // Thread for game ticks
  m_tickLoopThread = std::make_unique<std::thread>([this] { tickThread(); });

  // bind::bindEngine(m_scriptEngine);

  // Redirect the window events to the event manager
  proxyEvents();

  // Load core module
  loadModule("./data/core");

  // Инициализация всего остального
  EASY_BLOCK("Init event", profiler::colors::LightBlue)
  onInit(*this);

  m_debugInfoWindow = std::make_unique<DebugInfoWindow>(*this);
  m_settingsWindow = std::make_unique<SettingsWindow>(*this);
}

void px::Engine::loadCoreResources() {
  auto loadShader = [this](const std::string &id, const std::string &name) {
    const std::string vsPath = fmt::format("./data/core/shaders/{}/vs_{}.bin",
                                           Shader::getCurrentRendererTypeSuffix(), name);
    const std::string fsPath = fmt::format("./data/core/shaders/{}/fs_{}.bin",
                                           Shader::getCurrentRendererTypeSuffix(), name);

    std::string vsShaderCode    = io::loadFileToString(vsPath);
    const bgfx::Memory *vsMem   = utils::bgfx::stringToMemory(vsShaderCode);
    bgfx::ShaderHandle vsShader = bgfx::createShader(vsMem);

    std::string fsShaderCode    = io::loadFileToString(fsPath);
    const bgfx::Memory *fsMem   = utils::bgfx::stringToMemory(fsShaderCode);
    bgfx::ShaderHandle fsShader = bgfx::createShader(fsMem);

    auto shader = Ref<Shader>::make(vsShader, fsShader);
    m_registries->SHADERS.set(id, std::move(shader));
  };

  loadShader("core.cube", "cube");
  loadShader("core.imgui_image", "imgui_image");
  loadShader("core.imgui", "ocornut_imgui");
  loadShader("core.mesh", "mesh");

  auto loadFont = [this](const std::string &id, const std::string &name) {
    std::ifstream ifs(fmt::format("./data/core/font/{}.ttf", name));
    std::vector<char> data{std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()};
    m_registries->FONTS.set(id, std::move(data));
  };

  loadFont("core.roboto", "roboto/regular");
  loadFont("core.roboto.italic", "roboto/italic");
  loadFont("core.roboto.bold", "roboto/bold");

  auto loadLocalization = [this](const std::string &id, const std::string &name) {
    auto localization = Ref<Localization>::make();
    std::ifstream ifs(fmt::format("./data/core/lang/{}.yaml", name));
    localization->loadLanguage(ifs);
    m_registries->LOCALIZATIONS.set(id, std::move(localization));
  };

  loadLocalization("core.ru", "ru");
  loadLocalization("core.en", "en");

  m_registries->LOCALIZATIONS.copy("core", "core.ru");
}

// Registering default events
void px::Engine::registerEventTypes() {
  m_eventManager.registerEventClass<MouseEvent>();

  m_eventManager.registerEventClass<KeyEvent>();
  m_eventManager.registerEventClass<KeyReleasedEvent>();
  m_eventManager.registerEventClass<KeyPressedEvent>();
}

void px::Engine::proxyEvents() {
  if (m_window) {
    m_window->onMouseMoved.append([this](float x, float y) {
      m_eventManager.emplaceEvent<MouseEvent>(x, y);
    });

    m_window->onKey.append([this](KeyCode key, bool pressed, KeyModifiers::Enum mods) {
      m_eventManager.emplaceEvent<KeyEvent>(key, pressed);

      if (pressed)
        m_eventManager.emplaceEvent<KeyPressedEvent>(key);
      else
        m_eventManager.emplaceEvent<KeyReleasedEvent>(key);
    });
  }
}

void px::Engine::loop() {
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

void px::Engine::draw() {
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

void px::Engine::drawImGui() {
  EASY_BLOCK("px::Engine::drawImGui")
  m_imgui->BeginFrame(255);

  onGuiDraw();

  m_imgui->EndFrame();
}

void px::Engine::tickThread() {
  EASY_THREAD_SCOPE("Tick thread");
  FrameLimiter tickLimiter(m_maxTps);

  while (!m_tickThreadShouldStop) {
    tickLoop();
    m_tickDeltaTime = tickLimiter.sleep();
  }
}

void px::Engine::tickLoop() {
  EASY_BLOCK("px::Engine::tickLoop");

  m_eventManager.process();

  onPreTick(m_tickDeltaTime);
  onTick(m_tickDeltaTime);
  onPostTick(m_tickDeltaTime);
}

px::Window &px::Engine::getWindow() const {
  return *m_window;
}

px::EventManager &px::Engine::getEventManager() {
  return m_eventManager;
}

px::Controls &px::Engine::getControls() {
  return *m_controls;
}

px::Renderer &px::Engine::getRenderer() {
  return *m_renderer;
}

BS::thread_pool &px::Engine::getThreadPool() {
  return m_threadPool;
}

px::World *px::Engine::getWorld() const {
  return m_world.get();
}

px::Settings &px::Engine::getSettings() {
  return m_settings;
}

px::SettingsWindow &px::Engine::getSettingsWindow() {
  return *m_settingsWindow;
}

px::Registries &px::Engine::getRegistries() const {
  return *m_registries;
}

px::World &px::Engine::createNewWorld() {
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
