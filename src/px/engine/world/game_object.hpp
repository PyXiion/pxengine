#pragma once
#include <memory>
#include <list>
#include <string>
#include <vector>
#include "../components/component.hpp"

#define PX_OBJECT_NAME(name) constexpr inline static std::string_view defaultName = name;

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
    GameObject();
    virtual ~GameObject() = default;
    
    World &getWorld();

    /// @brief Установить имя (идентификатор) объекту.
    /// @param name 
    void setName(const std::string &name);
    [[nodiscard]] const std::string &getName() const;

    /// @brief Удалить объект из мира.
    void destroy();

    virtual void guiEditor();

    /// Create new component
    template <ComponentType T, typename ...TArgs>
    T *addComponent(TArgs &&...args);

    /// Get a component of a certain type
    /// \return Pointer to the component if successful, otherwise nullptr
    template <ComponentType T>
    T *getComponent();

    /// Get a list of components of a certain type
    /// \return A vector of components
    template <ComponentType T>
    std::vector<T *> getComponents();

    /// Remove a component of a certain type
    /// \return true if successful, otherwise false
    template <ComponentType T>
    bool removeComponent();

    /// Remove the component
    /// \param component Component to be removed
    /// \return true if successful, otherwise false
    bool removeComponent(Component *component);

    /// Remove a bunch of component of a certain type
    /// \return Amount of removed components
    template <ComponentType T>
    std::size_t removeComponents();

  private:
    px::Component::List m_components;

  private:
    std::string m_name;

    World *m_world;
    GameObjectIter m_self;
  };

  template<ComponentType T, typename... TArgs>
  T *GameObject::addComponent(TArgs &&... args) {
    // Allocate
    std::allocator<T> allocator;
    auto component = allocator.allocate(1);

    // Set the pointer to this game object (this)
    component->m_gameObject = this;

    // Call constructor
    std::construct_at(component, std::forward<TArgs>(args)...);

    // Push a shared ptr to vector and assign the iterator
    m_components.push_back(std::unique_ptr<T>(component));
    auto it = --m_components.end();
    component->m_handle = it;

    return component;
  }

  template<ComponentType T>
  T *GameObject::getComponent() {
    for (auto &component : m_components) {
      if (component->checkComponentType(T::componentTypeId)) {
        return component;
      }
    }
    return nullptr;
  }

  template<ComponentType T>
  std::vector<T *> GameObject::getComponents() {
    std::vector<T *> result;
    for (auto &component : m_components) {
      if (component->checkComponentType(T::componentTypeId)) {
        result.push_back(component.get());
      }
    }
    return result;
  }

  template<ComponentType T>
  bool GameObject::removeComponent() {
    if (m_components.empty())
      return false;

    auto it = std::find_if(m_components.begin(), m_components.end(),
                           [](auto &component) {
                             return component->checkComponentType(T::componentTypeId);
                           });
    bool success = it != m_components.end();

    if (success)
      m_components.erase(it);
    return success;
  }

  template<ComponentType T>
  std::size_t GameObject::removeComponents() {
    if (m_components.empty())
      return 0;

    std::size_t numRemoved{};

    auto it = m_components.begin();
    while (it != m_components.end()) {
      if ((*it)->checkComponentType(T::componentTypeId)) {
        it = m_components.erase(it);
        numRemoved += 1;
      } else {
        ++it;
      }
    }

    return numRemoved;
  }

}
