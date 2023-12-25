// Copyright (c) 2023.

//
// Created by pyxiion on 25.12.23.
//

#ifndef PX_ENGINE_SOME_MANAGER_HPP
#define PX_ENGINE_SOME_MANAGER_HPP
#include "px/script/template/names.hpp"

struct SomeManager {
  int speed = 30;

  void reset();
};

PX_AS_TYPENAME(SomeManager, "SomeManager")

#endif //PX_ENGINE_SOME_MANAGER_HPP
