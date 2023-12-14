// Copyright (c) 2023.

//
// Created by pyxiion on 13.12.23.
//

#ifndef PX_ENGINE_MODULE_BUILDER_HPP
#define PX_ENGINE_MODULE_BUILDER_HPP
#include <string>
#include <memory>
#include "module.hpp"

class CScriptBuilder;

namespace px::script {
  class AngelScript;

  class ModuleBuilder {
  public:
    explicit ModuleBuilder(AngelScript &as);
    ~ModuleBuilder();

    void startNewModule(const std::string &name);

    void addSectionFromFile(const std::string &filename);
    void addSection(const std::string &code, const std::string &sourcename);

    Module build();

  private:
    AngelScript &m_as;
    std::unique_ptr<CScriptBuilder> m_builder;

    std::string m_moduleName;
  };

} // px::script

#endif //PX_ENGINE_MODULE_BUILDER_HPP
