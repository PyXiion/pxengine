#include "game.hpp"
#include <functional>

px::Game::Game(Engine &engine)
  : m_engine(engine)
{
  m_engine.onInit.append(std::bind(&Game::onEngineInit, this, std::placeholders::_1));

  m_engine.onTick.append(std::bind(&Game::onEngineTick, this, std::placeholders::_1));
  m_engine.onUpdate.append(std::bind(&Game::onEngineUpdate, this, std::placeholders::_1));
}

void px::Game::onEngineInit(Engine &engine)
{
  ScriptEngine se = engine.getScriptEngine();

  m_module = se.getOrCreateModule("Game");
  m_module.loadScriptFromString("main.as", R"(
    void main()
    {
      println("Hello World!");
    }
  )");

  m_module.build();

  m_mainFunc = m_module.getFunctionByName("main");
}

void px::Game::onEngineExit(Engine &engine)
{
  
}

void px::Game::onEngineTick(float deltaTime)
{
  m_mainFunc();
}

void px::Game::onEngineUpdate(float deltaTime)
{
  
}