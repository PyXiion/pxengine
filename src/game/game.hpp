#pragma once
#include <engine/engine.hpp>
#include <engine/events/event_fabric.hpp>

namespace px
{
  class Game
  {
  public:
    Game(Engine &engine);

  private:
    Engine &m_engine;
    EventManager &m_eventManager;

    void onInit(Engine &engine);
    void onExit(Engine &engine);

    void onTick(float deltaTime);
    void onUpdate(float deltaTime);
  };
}