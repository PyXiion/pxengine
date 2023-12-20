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

  Module::~Module() {}

  asIScriptFunction *Module::getFunctionByDecl(const std::string &funcDecl) {
    return m_module->GetFunctionByDecl(funcDecl.c_str());
  }


} // px::script