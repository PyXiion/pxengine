// Copyright (c) 2024.

//
// Created by pyxiion on 01.01.24.
//

#ifndef PX_ENGINE_MODULE_HPP
#define PX_ENGINE_MODULE_HPP
#include <memory>
#include "px/px.hpp"
#include "px/engine/events/event_listener.hpp"

namespace px {
  namespace script {
    class Module;
  }

  class Engine;

  class Module : public EventListener {
  public:
    struct Meta {
      std::string name;

      struct {
        std::vector<std::string> sources;
      } scripts;
    };

  public:
    PX_NO_COPY(Module)

    Module(Engine &engine, std::string folder);

    void load();

    void afterLoad();

  private:
    void loadMeta();

    void init();

    const std::string &getScriptCode(const std::string &scriptId);

    Engine &m_engine;
    std::string m_folder;

    std::unique_ptr<script::Module> m_scriptModule;

    Meta m_meta;
  };

  typedef std::unique_ptr<Module> ModulePtr;

  template<class ...TArgs>
  decltype(auto) makeModule(TArgs && ...args) {
    return std::make_unique<Module>(std::forward<TArgs>(args) ...);
  }
} // px

#endif //PX_ENGINE_MODULE_HPP
