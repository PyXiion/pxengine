#pragma once
#include <list>

#include "game_object.hpp"

namespace px
{
  class Engine;
  class World
  {
    using GameObjectIter = std::list<GameObjectPtr>::const_iterator;

  public:
    World(Engine &engine);
    ~World();

    Engine &getEngine();

    template<class T = GameObject, class ...TArgs>
    GameObjectWeakPtr createGameObject(TArgs&&... args);

    void destroyObject(GameObjectIter &iterator);

  private:
    std::list<GameObjectPtr> m_gameObjects;
    Engine &m_engine;
  };
}

template<class T, class ...TArgs>
px::GameObjectWeakPtr px::World::createGameObject(TArgs&&... args)
{
  GameObjectPtr gameObject = std::make_shared<T>(*this, std::forward<TArgs>(args)...);

  return gameObject;
}
