// Copyright (c) 2023.

//
// Created by pyxiion on 11.11.23.
//
#include "component.hpp"
#include "../world/game_object.hpp"

const std::uint32_t px::Component::componentTypeId = std::hash<std::string>()("Component");

px::Component::Component() {
  m_gameObject = GameObject::currentGameObject;
}

bool px::Component::checkComponentType(std::uint32_t type) const {
  return componentTypeId == type;
}

std::uint32_t px::Component::getComponentTypeId() const {
  return componentTypeId;
}

px::GameObject *px::Component::getGameObject() {
  return m_gameObject;
}

const std::string_view &px::Component::getComponentId() const {
  static const std::string_view id = "component";
  return id;
}
