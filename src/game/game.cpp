#include "game.hpp"
#include <functional>

#include <easy/profiler.h>
#include "gameobjects/player.hpp"

px::Game::Game(Engine &engine)
  : m_engine(engine)
  , m_eventManager(m_engine.getEventManager())
{
  EASY_BLOCK("px::Game::Game");
  m_engine.onInit.append(std::bind(&Game::onInit, this, std::placeholders::_1));

  m_engine.onTick.append(std::bind(&Game::onTick, this, std::placeholders::_1));
  m_engine.onUpdate.append(std::bind(&Game::onUpdate, this, std::placeholders::_1));
}

void px::Game::onInit(Engine &engine)
{
  EASY_BLOCK("px::Game::onInit");

  auto &world = engine.createNewWorld();
  world.createGameObject<Player>()->setName("Potato");

  auto camera = world.createGameObject<Camera>();
  camera->setPosition(0, 0, 35.0f);
  camera->lookAt(0, 0, 36);

  camera->perspective(45, engine.getWindow().getAspect());

  engine.setCamera(camera);
}

void px::Game::onExit(Engine &engine)
{
  EASY_BLOCK("px::Game::onExit");
}

void px::Game::onTick(float deltaTime)
{
  EASY_BLOCK("px::Game::onTick");
}

void px::Game::onUpdate(float deltaTime)
{
  EASY_BLOCK("px::Game::onUpdate");
}