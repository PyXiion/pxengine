#include "engine.hpp"
#include <functional>
#include <utility>

#include <easy/profiler.h>
#include <bgfx/bgfx.h>
#include <imgui/imgui.h>
#include <bx/bx.h>

#include "common/frame_limiter.hpp"
#include "3d/cube.hpp"
#include <bx/bx.h>
#include <bx/math.h>

px::Engine::Engine()
  : m_maxFps(60)
  , m_maxTps(10)
  , m_speed(1.0f)
  , m_deltaTime(1.0f / float(m_maxFps))
  , m_tickDeltaTime(1.0f / float(m_maxTps))
  , m_tickThreadShouldStop(false)
  , m_debugInfoWindow(*this)
  , m_controls(*this)
  , m_showBgfxStats(false)
{
  instance = this;
}

px::Engine::~Engine()
{
}

void px::Engine::run()
{
  init();

  while (!m_window->isShouldClose())
  {
    loop();
  }
  m_window->close();

  // Завершение работы

  m_tickThreadShouldStop = true;
  m_tickLoopThread->join();

  onExit(*this);
}

void px::Engine::loadModule(const std::string &path)
{
}

void px::Engine::init()
{
  EASY_BLOCK("px::Engine::init", profiler::colors::Red);

  m_resourceManager = std::make_unique<ResourceManager>(*this, "./data");
  m_window = std::make_unique<Window>("PXE", 1280, 720);
  m_window->onKeyPressed.append([this](KeyCode keycode) {
    if (m_renderer) {
      m_renderer->setDebugEnabled(!m_renderer->isDebugEnabled());
    }
  });

  m_renderer = std::make_unique<Renderer>(*m_window);

  m_tickLoopThread = std::make_unique<std::thread>([this] { tickThread(); });

  // Инициализация всего остального
  onInit(*this);
}

void px::Engine::loop()
{
  EASY_BLOCK("px::Engine::loop");

  m_window->pollEvents();

  onPreUpdate(m_deltaTime);
  onUpdate(m_deltaTime);
  onPostUpdate(m_deltaTime);

  if (m_camera) {
    if (m_window->isKeyPressed(KeyCode_Up)) {
      m_camera->move(m_camera->getForward());
    } else if (m_window->isKeyPressed(KeyCode_Down)) {
      m_camera->move(-m_camera->getForward());
    } else if (m_window->isKeyPressed(KeyCode_Left)) {
      m_camera->move(m_camera->getRight());
    } else if (m_window->isKeyPressed(KeyCode_Right)) {
      m_camera->move(-m_camera->getRight());
    }
  }

  draw();
}

void px::Engine::draw() const
{
  static std::unique_ptr<Cube> cube;
  if (!cube)
    cube = std::make_unique<Cube>();

//  const bx::Vec3 at = {0.0f, 0.0f, 0.0f};
//  const bx::Vec3 eye = {0.0f, 0.0f, -35.0f};

//  {
//    int width, height;
//    std::tie(width, height) = m_window->size();
//
//    float view[16];
//    bx::mtxLookAt(view, eye, at);
//
//    float proj[16];
//    bx::mtxProj(proj, 60.0f, float(width)/float(height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
//    bgfx::setViewTransform(kClearView, view, proj);
//
//    bgfx::setViewRect(0, 0, 0, uint16_t(width), uint16_t(height) );
//  }

  if (m_camera) {
    m_camera->apply();
  }

  bgfx::touch(kClearView);

  bgfx::setDebug(m_showBgfxStats ? BGFX_DEBUG_STATS : BGFX_DEBUG_NONE);

  for (uint32_t yy = 0; yy < 11; ++yy)
  {
    for (uint32_t xx = 0; xx < 11; ++xx)
    {
      for (uint32_t zz = 0; zz < 11; ++zz) {
        float mtx[16];
        bx::mtxRotateXY(mtx, 0, 0);
        mtx[12] = -15.0f + float(xx) * 3.0f;
        mtx[13] = -15.0f + float(yy) * 3.0f;
        mtx[14] = -15.0f + float(zz) * 3.0f;

        // Set model matrix for rendering.
        bgfx::setTransform(mtx);

        // Set vertex and index buffer.
        cube->draw(0);
      }
    }
  }

  bgfx::frame();
}

void px::Engine::drawImGui()
{
  return;
//  m_renderer->beginImGuiFrame();
//  onGuiDraw();
//  m_renderer->renderImGui();
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
  EASY_BLOCK("Engine tick");

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
  return m_controls;
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

px::World &px::Engine::createNewWorld()
{
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
