// Copyright (c) 2023.

//
// Created by pyxiion on 25.12.23.
//

#ifndef PX_ENGINE_PLAYER_HPP
#define PX_ENGINE_PLAYER_HPP

#include <iostream>
#include "entity.hpp"

struct Player : public Entity {
  ~Player() override = default;

  void where() const final;

  const float maxHealth {5.0f};

  float x{};
  float y{};
};
typedef std::shared_ptr<Player> PlayerPtr;

PX_AS_TYPENAME          (Player, "Player");
PX_AS_TYPENAME_SMART_PTR(Player, "PlayerPtr");

#endif //PX_ENGINE_PLAYER_HPP
