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

    friend class px::GameObject;
    static thread_local World *current_world;

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
  EASY_BLOCK(__PRETTY_FUNCTION__)

  // Set the pointer to this world
  current_world = this;

  // Create object
  auto ptr = std::make_shared<T>(std::forward<TArgs>(args)...);

  // Append it to inner list
  {
    std::lock_guard lk(m_gameObjectsMutex);
    m_gameObjects.push_front(ptr);
    ptr->m_self = m_gameObjects.begin();
  }

  // Name it
  constexpr auto name = priv::getObjectDefaultName<T>();
  ptr->setName(fmt::format(name, ++unnamedUid));

  return ptr;
}
