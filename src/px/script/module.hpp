// Copyright (c) 2023.

//
// Created by pyxiion on 13.12.23.
//

#ifndef PX_ENGINE_SCRIPT_MODULE_HPP
#define PX_ENGINE_SCRIPT_MODULE_HPP

#include "px/px.hpp"
#include "common.hpp"
#include "function.hpp"
#include "object_type.hpp"
#include "exceptions.hpp"

namespace px::script {
  class Module {
  public:
    explicit Module(asIScriptContext *ctx, asIScriptModule *module);

    ~Module();

    template<FunctionSignature T>
    auto getFunction(const std::string &funcName) {
      const std::string decl = getSignature<T>(funcName);

      asIScriptFunction *funcHandle = getFunctionByDecl(decl);

      if (not funcHandle) {
        PX_THROW_AND_LOG("AngelScript", FunctionNotFound,
                         "Failed to get function {} by declaration \"{}\"", funcName, decl);
      }

      CVLOG(1, "AngelScript") << "Requested AngelScript function by declaration " << decl;
      return priv::GetFunctionType<T>(priv::FunctionHandle(m_ctx, funcHandle));
    }

    auto getType(const std::string &typeName) {
      return ObjectType(typeName, m_ctx, m_module);
    }

  private:
    asIScriptContext *m_ctx;
    asIScriptModule *m_module;

    asIScriptFunction *getFunctionByDecl(const std::string &funcDecl);
  };
} // px::script

#endif //PX_ENGINE_SCRIPT_MODULE_HPP
