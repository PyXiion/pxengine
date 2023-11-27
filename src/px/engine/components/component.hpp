// Copyright (c) 2023.

//
// Created by pyxiion on 11.11.23.
//

#ifndef ENGINE_COMPONENT_HPP
#define ENGINE_COMPONENT_HPP
#include <cstdint>
#include <string>
#include <list>
#include <memory>
#include <type_traits>

#define PX_COMPONENT_DECLARATION                                \
public:                                                         \
  static const std::size_t componentTypeId;                 \
  [[nodiscard]] virtual bool checkComponentType(std::size_t type) const override;

#define PX_COMPONENT_DEFINITION(parent, child)                                \
const std::size_t child::componentTypeId = std::hash<std::string>()(#child);  \
bool child::checkComponentType(std::size_t type) const {                      \
  return (componentTypeId == type) || parent::checkComponentType(type);       \
}

namespace px {
  class GameObject;

  class Component {
    friend class GameObject;
  public:

    Component();

    virtual ~Component() = default;

    virtual void guiEditor() {};

    [[nodiscard]] virtual bool checkComponentType(std::size_t type) const;
    static const std::size_t componentTypeId;

  protected:
    GameObject *getGameObject();

  private:
    using List = std::list<std::unique_ptr<px::Component>>;
    using Handle = List::iterator;

    Handle m_handle;
    GameObject *m_gameObject;
  };

  template <class T>
  concept ComponentType = std::is_base_of_v<Component, T>;

} // px

#endif //ENGINE_COMPONENT_HPP
