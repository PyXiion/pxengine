// Copyright (c) 2023.

//
// Created by pyxiion on 25.12.23.
//
#include <angelscript.h>
#include "binding.hpp"
#include "../engine.hpp"

PX_AS_TYPENAME(px::Engine, "Engine")

PX_AS_TYPENAME(px::ResourceManager, "ResourceManager")

PX_AS_TYPENAME(px::EventManager, "EventManager")

PX_AS_TYPENAME(px::Controls, "Controls")
PX_AS_TYPENAME(px::ControlAxis::Enum, "ControlAxis")

PX_AS_TYPENAME(px::World, "World")

PX_AS_TYPENAME(px::GameObjectPtr, "GameObject");

PX_AS_TYPENAME(px::Settings, "Settings")

namespace px::bind {

  void bindSettings(script::AngelScript &as) {
    as.registerObjectType<Settings *>();
  }

  void bindWorld(script::AngelScript &as) {
    as.registerObjectType<World *>()
        .method<&World::getObjectByName>("getObjectByName");
//        .method<&World::getAllGameObjects>("getAllGameObjects");
  }

  void bindGameObject(script::AngelScript &as) {
    as.registerObjectType<GameObjectPtr>();
  }

  void bindControls(script::AngelScript &as) {
    auto engine = as.getHandle();

    // TODO AngelScript::registerEnum
    engine->RegisterEnum("ControlAxis");

    engine->RegisterEnumValue("ControlAxis", "Vertical", ControlAxis::Vertical);
    engine->RegisterEnumValue("ControlAxis", "Horizontal", ControlAxis::Horizontal);

    engine->RegisterEnumValue("ControlAxis", "Jump", ControlAxis::Jump);

    engine->RegisterEnumValue("ControlAxis", "MouseX", ControlAxis::MouseX);
    engine->RegisterEnumValue("ControlAxis", "MouseY", ControlAxis::MouseY);

    as.registerObjectType<Controls *>()
        .method<&Controls::getAxis>("getAxis");
  }

  void bindEventManager(script::AngelScript &as) {
    as.registerObjectType<EventManager *>();
  }

  void bindResourceManager(script::AngelScript &as) {
    as.registerObjectType<ResourceManager *>();
  }

  void bindConsole(script::AngelScript &as) {
    auto info = static_cast<void(*)(const std::string&)>([](const std::string &msg) {
      CLOG(INFO, "PXEngine") << msg;
    });

    as.registerGlobalFunction("info", info);
  }

  void bindEngine(script::AngelScript &as) {
    bindConsole(as);

    bindSettings(as);
    bindGameObject(as);
    bindWorld(as);
    bindControls(as);
    bindEventManager(as);
    bindResourceManager(as);

    as.registerObjectType<Engine *>()
        .method<&Engine::reloadSettings>("reloadSettings")

        .method<&Engine::getResourceManager>("getResourceManager")
        .method<&Engine::getEventManager>("getEventManager")
        .method<&Engine::getControls>("getControls")
        .method<&Engine::getWorld>("getWorld")
        .method<&Engine::getSettings>("getSettings");

    as.registerGlobalVariable("engine", &Engine::getInstance());
  }
}