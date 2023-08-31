#pragma once
#include <memory>
#include <list>
#include <string>

namespace px
{
  class World;
  class GameObject;

  typedef std::shared_ptr<GameObject> GameObjectPtr;
  typedef std::weak_ptr<GameObject> GameObjectWeakPtr;

  class GameObject
  {
    friend class World;
    using GameObjectIter = std::list<GameObjectPtr>::const_iterator;

  public:
    const std::string &getName() const;
    World &getWorld();

    void setName(const std::string &name);

    void destroy();

  protected:
    GameObject(World &world) : m_world(world) {}
    virtual ~GameObject() = default;

  private:

    std::string m_name;

    World &m_world;
    GameObjectIter m_self;
  };

}
