// Copyright (c) 2023.

//
// Created by pyxiion on 13.12.23.
//

#ifndef PX_ENGINE_MODULE_HPP
#define PX_ENGINE_MODULE_HPP
#include "common.hpp"
#include "function.hpp"
#include "object_type.hpp"
#include "template/signatures.hpp"

namespace px::script {

  class Module {
  public:
    explicit Module(asIScriptContext *ctx, asIScriptModule *module);
    ~Module();

    template<class TReturn, class ...TArgs>
    auto getFunction(const std::string &funcName) {
      const std::string decl = getSignature<TReturn (TArgs...)>(funcName);

      asIScriptFunction *funcHandle = getFunctionByDecl(decl);


      if (not funcHandle) {
        CLOG(ERROR, "AngelScript") << "Failed to get a function by declaration " << decl;
        throw std::runtime_error(fmt::format("Failed to get function {} by declaration {}.", funcName, decl));
      }
      CVLOG(1, "AngelScript") << "Requested AngelScript function by declaration " << decl;
      return Function<TReturn (typename priv::GetTypeAsNameImpl<TArgs>::Type...)>(priv::FunctionHandle(m_ctx, funcHandle));
    }

    auto getType(const std::string &typeName) {
      return ObjectType(typeName, m_ctx, m_module);
    }

  private:
    asIScriptContext *m_ctx;
    asIScriptModule  *m_module;

    asIScriptFunction *getFunctionByDecl(const std::string &funcDecl);
  };

} // px::script

#endif //PX_ENGINE_MODULE_HPP
