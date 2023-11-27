// Copyright (c) 2023.

//
// Created by pyxiion on 11.11.23.
//
#include "component.hpp"
#include "../world/game_object.hpp"

const std::size_t px::Component::componentTypeId = std::hash<std::string>()("Component");

px::Component::Component() {
  m_gameObject = px::GameObject::currentGameObject;
}

bool px::Component::checkComponentType(std::size_t type) const {
  return componentTypeId == type;
}

px::GameObject *px::Component::getGameObject() {
  return m_gameObject;
}
