#pragma once
#include <unordered_map>
#include <list>
#include <string>
#include <fmt/format.h>
#include <easy/profiler.h>

#include "game_object.hpp"

namespace px
{
  class Engine;
  class World
  {
    friend class GameObject;
    using GameObjectIter = std::list<GameObjectPtr>::iterator;

  public:
    World(Engine &engine);
    ~World();

    Engine &getEngine();

    template<class T = GameObject, class ...TArgs>
    GameObjectPtr createGameObject(TArgs... args);
    GameObjectPtr getObjectByName(const std::string &name);

    const std::list<GameObjectPtr> &getAllGameObjects() const;

  private:
    std::unordered_map<std::string, GameObjectWeakPtr> m_gameObjectsByName;
    std::list<GameObjectPtr> m_gameObjects;
    Engine &m_engine;
    int unnamedUid = 0;

    void updateObjectName(GameObjectPtr gameobject, const std::string newName);
    void destroyObject(GameObjectIter &iterator);
  };
}

template<class T, class ...TArgs>
px::GameObjectPtr px::World::createGameObject(TArgs... args)
{
  EASY_BLOCK(__PRETTY_FUNCTION__);
  px::GameObjectPtr gameObject = std::make_shared<T>(*this, std::forward<TArgs>(args)...);

  m_gameObjects.push_front(gameObject);
  gameObject->m_self = m_gameObjects.begin();
  gameObject->setName(fmt::format("Unnamed object {}", ++unnamedUid));

  return gameObject;
}
