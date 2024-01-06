// Copyright (c) 2023.

//
// Created by pyxiion on 26.12.23.
//

#include "angel_behaviour.hpp"
#include "px/engine/engine.hpp"

namespace px {
  std::unique_ptr<script::ObjectType> AngelBehaviour::baseAngelBehaviourType;
  std::once_flag AngelBehaviour::staticInit;

  void AngelBehaviour::init() {
    auto &engine = Engine::getInstance();
    auto &as = engine.getScriptEngine();
    auto module = as.getExistingModule("core/angel_behaviour.as");

    baseAngelBehaviourType = std::make_unique<script::ObjectType>(module.getType("AngelBehaviour"));
    CLOG(INFO, "PXEngine") << "AngelBehaviour has been initialised.";
  }

  AngelBehaviour::AngelBehaviour() {
    call_once(staticInit, init);
  }

  AngelBehaviour::AngelBehaviour(const std::string &scriptId) {
    call_once(staticInit, init);
  }
} // px