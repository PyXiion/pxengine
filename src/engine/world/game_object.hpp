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
    using GameObjectIter = std::list<GameObjectPtr>::iterator;

  public:
    virtual ~GameObject() = default;
    
    World &getWorld();

    /// @brief Установить имя (идентификатор) объекту.
    /// @param name 
    void setName(const std::string &name);
    [[nodiscard]] const std::string &getName() const;

    /// @brief Удалить объект из мира.
    void destroy();

  private:
    std::string m_name;

    World *m_world;
    GameObjectIter m_self;
  };

}
