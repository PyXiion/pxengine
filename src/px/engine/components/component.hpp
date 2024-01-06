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
#include <px/templates.hpp>
#include <px/utils/constexpr_hash.hpp>

namespace px {
  class GameObject;

  class Component {
    friend class GameObject;
  public:

    Component();

    virtual ~Component() = default;

    virtual void guiEditor() {}

    [[nodiscard]] virtual bool checkComponentType(std::uint32_t type) const;
    [[nodiscard]] virtual const std::string_view &getComponentId() const;
    static const std::uint32_t componentTypeId;

  protected:
    GameObject *getGameObject();

  private:
    using List = std::list<std::unique_ptr<px::Component>>;
    using Handle = List::iterator;

    Handle m_handle;
    GameObject *m_gameObject;
  };

  template <class T>
  concept ComponentType = std::is_base_of_v<Component, T> or std::is_same_v<Component, T>;

  template<px::string_literal ID, ComponentType TParent = Component>
  class BaseComponent : public TParent {
  public:
    virtual ~BaseComponent() = default;
    inline static constexpr std::uint32_t componentTypeId = px::crc32(ID.value);
    inline static constexpr std::string_view componentId = ID.value;

    [[nodiscard]] bool checkComponentType(std::uint32_t type) const override {
      return componentTypeId == type or TParent::checkComponentType(type);
    }
    [[nodiscard]] const std::string_view &getComponentId() const override {
      return componentId;
    }
  };

} // px

#endif //ENGINE_COMPONENT_HPP
