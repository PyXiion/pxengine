//
// Created by pyxiion on 13.12.23.
//

#include "module_builder.hpp"
#include <cassert>
#include "angel_script.hpp"
#include "addons/scriptbuilder/scriptbuilder.h"

namespace px::script {
  ModuleBuilder::ModuleBuilder(AngelScript &as)
      : m_as(as)
      , m_builder(std::make_unique<CScriptBuilder>()) {

  }
  ModuleBuilder::~ModuleBuilder() = default;

  void ModuleBuilder::startNewModule(const std::string &name) {
    CLOG(INFO, "AngelScript") << "Building AngelScript module " << name;

    m_moduleName = name;
    assert(m_builder->StartNewModule(m_as.getHandle(), name.c_str()) >= 0);
  }

  void ModuleBuilder::addSectionFromFile(const std::string &filename) {
    CLOG(INFO, "AngelScript") << "Loading " << filename << " to AngelScript module " << m_moduleName;
    assert(m_builder->AddSectionFromFile(filename.c_str()) >= 0);
  }

  void ModuleBuilder::addSection(const std::string &code, const std::string &sourcename) {
    CLOG(INFO, "AngelScript") << "Loading a code to AngelScript module " << m_moduleName;
    assert(m_builder->AddSectionFromMemory(sourcename.c_str(), code.c_str()));
  }

  Module ModuleBuilder::build() {
    int r = m_builder->BuildModule();
    if (r < 0) {
      CLOG(ERROR, "AngelScript") << "Failed to build the AngelScript module " << m_moduleName;
      throw std::runtime_error("Failed to build AS module.");
    }
    CLOG(INFO, "AngelScript") << "Successfully builded the AngelScript module " << m_moduleName;
    return Module(m_as.getContext(), m_as.getHandle()->GetModule(m_moduleName.c_str()));
  }
} // px::script