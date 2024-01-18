// Copyright (c) 2024.

//
// Created by pyxiion on 01.01.24.
//

#include "module.hpp"
#include <filesystem>
#include <algorithm>
#include <utility>
#include <fmt/format.h>
#include <yaml-cpp/yaml.h>
#include "easylogging++.h"
#include "engine.hpp"

namespace fs = std::filesystem;

namespace px {
  Module::Module(Engine &engine, std::string folder)
    : m_engine(engine)
  , m_folder(std::move(folder)) {}

  void Module::load() {
    loadMeta();

    init();
  }

  void Module::afterLoad() {
    // bind script functions
    if (m_scriptModule)
      m_scriptModule->bindImported();
  }

  void Module::loadMeta() {
    CLOG(INFO, "PXEngine") << "Loading a module in \"" << m_folder << "\"...";
    const auto possibleMetaFiles = {
      "meta.yml", "meta.yaml", "meta.json"
    };
    auto makePath = [this](const std::string &f) { return fs::path(m_folder) / f; };
    auto exists   = [](const std::string &f) -> bool { return fs::exists(f); };

    auto possiblePaths = possibleMetaFiles | std::views::transform(makePath);
    auto metaFilename  = std::ranges::find_if(possiblePaths, exists);

    if (metaFilename == possiblePaths.end()) {
      PX_THROW_AND_LOG("PXEngine", std::runtime_error, "Metafile not found (tried {})",
                       fmt::join(possibleMetaFiles, " "));
    }

    CVLOG(1, "PXEngine") << "Found meta file: " << *metaFilename;

    auto meta = YAML::LoadFile(*metaFilename);

    m_meta.name = meta["name"].as<std::string>();

    if (auto scripts         = meta["scripts"]) {
      if (const auto sources = scripts["sources"])
        m_meta.scripts.sources = sources.as<std::vector<std::string>>();
    }

    CLOG(INFO, "PXEngine") << "Module name: " << m_meta.name;
  }

  void Module::init() {
    const auto &sources = m_meta.scripts.sources;
    if (sources.empty())
      return;

    auto &scriptEngine = m_engine.getScriptEngine();

    auto builder = scriptEngine.createModuleBuilder();
    builder.begin(m_meta.name);

    auto rootId = fs::path(m_folder).filename().string() + ".";
    auto &rm    = m_engine.getResourceManager();

    CLOG(INFO, "PXEngine") << "The order of scripts:";
    for (const auto &sourceId : sources) {
      CLOG(INFO, "PXEngine") << "\t- " << sourceId;
    }
    for (const auto &sourceId : sources) {
      auto id      = rootId + sourceId;
      auto codePtr = rm.get<std::string>(id);
      builder.code(*codePtr, id);
    }

    auto module = builder.build();

    // void start()
    try {
      auto start         = module.getFunction<void()>("start");
      auto startCallback = [start](px::Engine &) mutable { start(); };
      listen(m_engine.onInit, startCallback);
    } catch (const px::script::FunctionNotFound &) {
      CVLOG(1, "PXEngine") << "Module scripts have no start function";
    }

    // void update()
    try {
      auto update         = module.getFunction<void()>("update");
      auto updateCallback = [update](float) mutable { update(); };
      listen(m_engine.onUpdate, updateCallback);
    } catch (const px::script::FunctionNotFound &) {
      CVLOG(1, "PXEngine") << "Module scripts have no update function";
    }

    // void shutdown()
    try {
      auto shutdown         = module.getFunction<void()>("shutdown");
      auto shutdownCallback = [shutdown](px::Engine &) mutable { shutdown(); };
      listen(m_engine.onExit, shutdownCallback, true);
    } catch (const px::script::FunctionNotFound &) {
      CVLOG(1, "PXEngine") << "Module scripts have no shutdown function";
    }

    m_scriptModule = std::make_unique<script::Module>(std::move(module));
  }

  const std::string &Module::getScriptCode(const std::string &scriptId) {
    return *m_engine.getResourceManager().get<std::string>(scriptId);
  }
} // px
