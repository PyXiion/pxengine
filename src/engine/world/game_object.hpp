#pragma once
#include <memory>

#include "../common/vector3.hpp"

namespace px
{
  class World;
  class GameObject
  {
    friend class World;

    GameObject(World &world) : m_world(world) {}
    virtual ~GameObject() = default;

  public:
    World &getWorld();

  private:
    World &m_world;
  };

  typedef std::shared_ptr<GameObject> GameObjectPtr;
  typedef std::weak_ptr<GameObject> GameObjectWeakPtr;
}
