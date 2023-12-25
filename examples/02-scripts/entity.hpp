// Copyright (c) 2023.

//
// Created by pyxiion on 25.12.23.
//

#ifndef PX_ENGINE_ENTITY_HPP
#define PX_ENGINE_ENTITY_HPP
#include <memory>
#include "px/script/template/names.hpp"

struct Entity {
  virtual ~Entity() = default;

  float health {30.0f};

  virtual void where() const = 0;
};
typedef std::shared_ptr<Entity> EntityPtr;

PX_AS_TYPENAME          (Entity, "Entity");
PX_AS_TYPENAME_SMART_PTR(Entity, "EntityPtr");

#endif //PX_ENGINE_ENTITY_HPP
