// Copyright (c) 2023.

//
// Created by pyxiion on 13.12.23.
//

#include "module.hpp"
#include <angelscript.h>

namespace px::script {
  Module::Module(asIScriptContext *ctx, asIScriptModule *module)
      : m_ctx(ctx)
      , m_module(module) {
  }

  Module::~Module() = default;

  ObjectType Module::getType(const std::string &typeName) {
    return {typeName, m_ctx, m_module};
  }

  ObjectType Module::getTypeByIndex(unsigned index) {
    return {index, m_ctx, m_module};
  }

  unsigned Module::getObjectTypeCount() const {
    return m_module->GetObjectTypeCount();
  }

  void Module::bindImported() {
    m_module->BindAllImportedFunctions();
  }

  asIScriptFunction *Module::getFunctionByDecl(const std::string &funcDecl) {
    return m_module->GetFunctionByDecl(funcDecl.c_str());
  }


} // px::script