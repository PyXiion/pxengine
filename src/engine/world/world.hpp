#pragma once
#include <list>

#include "game_object.hpp"

namespace px
{
  class Engine;
  class World
  {
  public:
    World(Engine &engine);
    ~World();

    Engine &getEngine();

    template<class T = GameObject, class ...TArgs>
    GameObjectWeakPtr createGameObject(TArgs&&... args);

  private:
    std::list<GameObjectPtr> m_gameObjects;
    Engine &m_engine;
  };
}

template<class T, class ...TArgs>
px::GameObjectWeakPtr px::World::createGameObject(TArgs&&... args)
{
  GameObjectPtr gameObject = std::make_shared<T>(std::forward<TArgs>(args)...);

  return gameObject;
}
