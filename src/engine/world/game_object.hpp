#pragma once
#include <memory>
#include <list>

#include "../math/vector.hpp"

namespace px
{
  class World;
  class GameObject;

  typedef std::shared_ptr<GameObject> GameObjectPtr;
  typedef std::weak_ptr<GameObject> GameObjectWeakPtr;

  class GameObject
  {
    using GameObjectIter = std::list<GameObjectPtr>::const_iterator;

  private:
    friend class World;

    GameObject(World &world) : m_world(world) {}
    virtual ~GameObject() = default;

  public:
    void destroy();

    World &getWorld();

  private:
    World &m_world;
    GameObjectIter m_iter;
  };

}
