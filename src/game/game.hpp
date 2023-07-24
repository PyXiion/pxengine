#pragma once
#include <engine/engine.hpp>

namespace px
{
  class Game
  {
  public:
    Game(Engine &engine);


  private:
    Engine &m_engine;

    ScriptModule m_module;
    ScriptFunction m_mainFunc;

    void onEngineInit(Engine &engine);
    void onEngineExit(Engine &engine);

    void onEngineTick(float deltaTime);
    void onEngineUpdate(float deltaTime);
  };
}