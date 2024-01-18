//
// Created by pyxiion on 13.12.23.
//

#include "module_builder.hpp"
#include "angel_script.hpp"
#include "addons/scriptbuilder/scriptbuilder.h"

namespace px::script {
  ModuleBuilder::ModuleBuilder(AngelScript &as)
      : m_as(as)
      , m_builder(std::make_unique<CScriptBuilder>()) {

  }
  ModuleBuilder::~ModuleBuilder() = default;

  ModuleBuilder &ModuleBuilder::begin(const std::string &name) {
    CLOG(INFO, "AngelScript") << "Building AngelScript module " << name << "...";

    m_moduleName = name;

    if (m_builder->StartNewModule(m_as.getHandle(), name.c_str()) < 0) {
      PX_THROW_AND_LOG("AngelScript", std::runtime_error, "Failed to create new module named {}", name);
    }

    return *this;
  }

  ModuleBuilder &ModuleBuilder::codeFromFile(const std::string &filename) {
    CLOG(INFO, "AngelScript") << "Loading " << filename << " to AngelScript module " << m_moduleName;

    if (m_builder->AddSectionFromFile(filename.c_str()) < 0) {
      PX_THROW_AND_LOG("AngelScript", std::runtime_error, "Failed to load script from \"{}\"", filename);
    }

    return *this;
  }

  ModuleBuilder &ModuleBuilder::code(const std::string &code, const std::string &sourceName) {
    CLOG(INFO, "AngelScript") << "Loading a code to AngelScript module " << m_moduleName;

    if (m_builder->AddSectionFromMemory(sourceName.c_str(), code.c_str()) < 0) {
      PX_THROW_AND_LOG("AngelScript", std::runtime_error, "Failed to load code (sourceName: {})", sourceName);
    }

    return *this;
  }

  ModuleBuilder &ModuleBuilder::end() {
    build();
    return *this;
  }

  Module ModuleBuilder::build() {
    int r = m_builder->BuildModule();
    if (r < 0) {
      CLOG(ERROR, "AngelScript") << "Failed to build an AngelScript module " << m_moduleName;
      throw std::runtime_error("Failed to build an AS module.");
    }
    CLOG(INFO, "AngelScript") << "Successfully builded an AngelScript module " << m_moduleName;

    return Module(m_as.getContext(), m_as.getHandle()->GetModule(m_moduleName.c_str()));
  }
} // px::script