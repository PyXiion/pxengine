// Copyright (c) 2023.

//
// Created by pyxiion on 11.12.23.
//

#ifndef PX_ENGINE_ANGEL_SCRIPT_HPP
#define PX_ENGINE_ANGEL_SCRIPT_HPP
#include <px/px.hpp>
#include "common.hpp"
#include "template/signatures.hpp"
#include "module_builder.hpp"
#include "object_type_builder.hpp"
#include "easylogging++.h"

namespace px::script {

  class AngelScript {
  public:
    AngelScript();
    ~AngelScript();

    template<class T>
    void registerGlobalVariable(std::string_view &&name, T *ptr);

    template<FunctionSignature T>
    void registerGlobalFunction(std::string_view &&name, T *fun);

    template<FunctionSignature T>
    void registerGlobalFunctionWithDecl(const std::string &decl, T *&&fun);

    template<class T>
    AsTypeBuilder<T> registerObjectType();

    template<class T>
    AsTypeBuilder<T> registerObjectType(std::string &&name);

    asIScriptEngine *getHandle();
    asIScriptContext *getContext();

    ModuleBuilder createModuleBuilder();
    Module getExistingModule(const std::string &moduleName);

  private:
    asIScriptEngine  *m_handle;
    asIScriptContext *m_ctx;

    static void messageCallback(const asSMessageInfo *msg, void *param);

    void init();

    void registerGlobalFunction(const std::string &funSign, void *funPtr);
    void registerGlobalVariable(const std::string &signature, void *ptr);
  };

  template<FunctionSignature T>
  void AngelScript::registerGlobalFunction(std::string_view &&name, T *fun) {
    auto signature = getSignature<T>(std::forward<decltype(name)>(name));

    registerGlobalFunction(signature, PX_ANYTHING_TO_VOID_PTR(fun));
    CLOG(INFO, "AngelScript") << "Registered an AngelScript global function with a signature \"" << signature <<
                              "\" and function pointer at " << PX_ANYTHING_TO_VOID_PTR(fun);
  }

  template<FunctionSignature T>
  void AngelScript::registerGlobalFunctionWithDecl(const std::string &decl, T *&&fun) {
    registerGlobalFunction(decl, PX_ANYTHING_TO_VOID_PTR(fun));
  }

  template<class T>
  AsTypeBuilder<T> AngelScript::registerObjectType() {
    return AsTypeBuilder<T>(m_handle);
  }

  template<class T>
  AsTypeBuilder<T> AngelScript::registerObjectType(std::string &&name) {
    return AsTypeBuilder<T>(m_handle, std::forward<decltype(name)>(name));
  }

  template<class T>
  void AngelScript::registerGlobalVariable(std::string_view &&name, T *ptr) {
    auto &&typeName = getTypeAsName<T>();
    std::string signature = fmt::format("{} {}", std::forward<decltype(typeName)>(typeName), std::forward<decltype(name)>(name));
    registerGlobalVariable(signature, PX_ANYTHING_TO_VOID_PTR(ptr));
    CLOG(INFO, "AngelScript") << "New global variable \"" << signature << "\" at " << PX_ANYTHING_TO_VOID_PTR(ptr);
  }

} // px::script

#endif //PX_ENGINE_ANGEL_SCRIPT_HPP
