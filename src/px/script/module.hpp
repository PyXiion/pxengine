// Copyright (c) 2023.

//
// Created by pyxiion on 13.12.23.
//

#ifndef PX_ENGINE_MODULE_HPP
#define PX_ENGINE_MODULE_HPP
#include "common.hpp"
#include "function.hpp"
#include "template/as_function.hpp"

namespace px::script {

  class Module {
  public:
    explicit Module(asIScriptContext *ctx, asIScriptModule *module);
    ~Module();

    template<class TReturn, class ...TArgs>
    auto getFunction(const std::string &funcName) {
      const std::string decl = AsFunction<TReturn (TArgs...)>::getSignature(funcName);

      asIScriptFunction *funcHandle = getFunctionByDecl(decl);

      if (not funcHandle) {
        throw std::runtime_error("Failed to get function by declaration.");
      }

      return Function<TReturn (TArgs...)>(priv::FunctionHandle(m_ctx, funcHandle));
    }

  private:
    asIScriptContext *m_ctx;
    asIScriptModule  *m_module;

    asIScriptFunction *getFunctionByDecl(const std::string &funcDecl);
  };

} // px::script

#endif //PX_ENGINE_MODULE_HPP
