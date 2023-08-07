#include "engine.hpp"
#include <functional>

#include <easy/profiler.h>
#include <imgui/imgui.h>

#include "graphics/gl/renderer.hpp"
#include "common/frame_limiter.hpp"

px::Engine::Engine()
  : m_maxFps(60)
  , m_maxTps(10)
  , m_speed(1.0f)
  , m_deltaTime(1.0f / m_maxFps)
  , m_tickDeltaTime(1.0f / m_maxTps)
  , m_tickThreadShouldStop(false)
{
}

px::Engine::~Engine()
{
}

void px::Engine::run()
{
  init();

  FrameLimiter fpsLimiter(60);
  while (!m_window->isShouldClose())
  {
    loop();

    m_deltaTime = fpsLimiter.sleep();
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

  m_window = std::make_unique<Window>("PXE", 800, 600);

  // TODO выбор между OpenGL или Vulkan
  if (true)
  {
    m_renderer = std::make_unique<gl::Renderer>(*m_window);
  }
  else
  {
    // ...
  }

  m_renderer->initImGui();

  m_window->onFramebufferResize.append([this](int width, int height)
  {
    m_renderer->setViewportSize(0, 0, width, height);
  });

  m_scriptEngine = std::make_unique<ScriptEngine>();
  m_scriptEngine->bindAll();

  m_tickLoopThread = std::make_unique<std::thread>(std::bind(&Engine::tickThread, this));

  // Инициализация всего остального
  onInit(*this);

  // test
  onGuiDraw.append([]() {
    ImGui::Begin("Test window");
      ImGui::Text("Hello world");
    ImGui::End();
  });
}

void px::Engine::loop()
{
  EASY_BLOCK("Engine draw update");

  m_window->pollEvents();
  m_renderer->clear();

  onUpdate(m_deltaTime * m_speed);

  draw();

  m_renderer->swapBuffers();
}

void px::Engine::draw()
{
  drawImGui();
}

void px::Engine::drawImGui()
{
  m_renderer->beginImGuiFrame();
  onGuiDraw();
  m_renderer->renderImGui();
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
  float deltaTime = m_tickDeltaTime * m_speed; 

  m_eventManager.process();

  onPreTick(deltaTime);
  onTick(deltaTime);
  onPostTick(deltaTime);
}


px::Window &px::Engine::getWindow()
{
  return *m_window;
}
px::Renderer &px::Engine::getRenderer()
{
  return *m_renderer;
}
px::ScriptEngine &px::Engine::getScriptEngine()
{
  return *m_scriptEngine;
}

px::EventManager &px::Engine::getEventManager()
{
  return m_eventManager;
}
