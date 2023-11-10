#pragma once
#include <unordered_map>
#include <list>
#include <string>
#include <mutex>
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
    explicit World(Engine &engine);
    ~World();

    Engine &getEngine();

    template<class T = GameObject, class ...TArgs>
      requires std::is_base_of_v<px::GameObject, T>
    std::shared_ptr<T> createGameObject(TArgs... args);

    GameObjectPtr getObjectByName(const std::string &name);

    const std::list<GameObjectPtr> &getAllGameObjects() const;

  private:
    std::unordered_map<std::string, GameObjectWeakPtr> m_gameObjectsByName;
    std::list<GameObjectPtr> m_gameObjects;
    Engine &m_engine;
    int unnamedUid = 0;

    std::mutex m_gameObjectsMutex;

    void updateObjectName(GameObjectPtr &gameObject, const std::string &newName);
    void destroyObject(GameObjectIter &iterator);
  };

  namespace priv {
    template <class T>
    concept HasDefaultName = requires {
      { T::defaultName } -> std::convertible_to<std::string_view>;
    };

    template <class T>
    inline constexpr std::string_view getObjectDefaultName() {
      if constexpr (HasDefaultName<T>)
        return T::defaultName;
      else
        return "Unnamed object #{}";
    }
  }
}

template<class T, class ...TArgs>
  requires std::is_base_of_v<px::GameObject, T>
std::shared_ptr<T> px::World::createGameObject(TArgs... args)
{
  EASY_BLOCK(__PRETTY_FUNCTION__);

  // Allocate enough space
  void *raw = ::operator new(sizeof(T));

  // Get a GameObject ptr
  auto gameObjectPtr = reinterpret_cast<GameObject *>(raw);
  // Set pointer to the world (this)
  gameObjectPtr->m_world = this;

  // Call constructor
  new (raw) T(std::forward<TArgs>(args)...);

  // Create a shared pointer.
  auto tPtr = reinterpret_cast<T*>(raw);
  auto gameObject = std::shared_ptr<T>(tPtr);

  // Append it to inner list
  {
    std::lock_guard lk(m_gameObjectsMutex);
    m_gameObjects.push_front(gameObject);
    gameObject->m_self = m_gameObjects.begin();
  }

  constexpr auto name = priv::getObjectDefaultName<T>();

  // Name it
  gameObject->setName(fmt::format(name, ++unnamedUid));

  return gameObject;
}
