// Copyright (c) 2023.

//
// Created by pyxiion on 11.12.23.
//

#ifndef PX_ENGINE_ANGEL_SCRIPT_HPP
#define PX_ENGINE_ANGEL_SCRIPT_HPP
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

    template<class T, class ...U>
    inline void registerGlobalFunction(std::string_view &&name, T (*ptr)(U...));

    template<class T, class ...U>
    inline void registerGlobalFunctionWithDecl(const std::string &decl, T (*ptr)(U...));

    template<class T>
    inline AsTypeBuilder<T> registerObjectType();
    template<class T>
    inline AsTypeBuilder<T> registerObjectType(std::string &&name);

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
  };

  template<class T, class ...U>
  void AngelScript::registerGlobalFunction(std::string_view &&name, T (*ptr)(U...)) {
    auto signature = getSignature<T (U...)>(std::forward<decltype(name)>(name));

    registerGlobalFunction(signature, reinterpret_cast<void *>(ptr));
    CLOG(INFO, "AngelScript") << "Registered an AngelScript global function with a signature \"" << signature <<
                              "\" and function pointer at " << reinterpret_cast<void*>(ptr);
  }

  template<class T, class... U>
  void AngelScript::registerGlobalFunctionWithDecl(const std::string &decl, T (*ptr)(U...)) {
    registerGlobalFunction(decl, reinterpret_cast<void *>(ptr));
  }

  template<class T>
  AsTypeBuilder<T> AngelScript::registerObjectType() {
    return AsTypeBuilder<T>(m_handle);
  }
  template<class T>
  AsTypeBuilder<T> AngelScript::registerObjectType(std::string &&name) {
    return AsTypeBuilder<T>(m_handle, std::forward<decltype(name)>(name));
  }

} // px::script

#endif //PX_ENGINE_ANGEL_SCRIPT_HPP
