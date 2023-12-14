// Copyright (c) 2023.

//
// Created by pyxiion on 11.12.23.
//

#ifndef PX_ENGINE_ANGEL_SCRIPT_HPP
#define PX_ENGINE_ANGEL_SCRIPT_HPP
#include "common.hpp"
#include "template/as_function.hpp"
#include "module_builder.hpp"

namespace px::script {

  class AngelScript {
  public:
    AngelScript();
    ~AngelScript();

    template<class T, class ...U>
    inline void registerGlobalFunction(std::string_view name, T (*ptr)(U...));

    template<class T, class ...U>
    inline void registerGlobalFunctionWithDecl(const std::string &decl, T (*ptr)(U...));

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

  template<class T, class... U>
  void AngelScript::registerGlobalFunction(std::string_view name, T (*ptr)(U...)) {
    auto f = AsFunction<T (U...)>{std::forward<typename AsFunction<T (U...)>::FunctionPtr>(ptr)};

    registerGlobalFunction(f.getSignature(name), reinterpret_cast<void *>(ptr));
  }

  template<class T, class... U>
  void AngelScript::registerGlobalFunctionWithDecl(const std::string &decl, T (*ptr)(U...)) {
    registerGlobalFunction(decl, reinterpret_cast<void *>(ptr));
  }

} // px::script

#endif //PX_ENGINE_ANGEL_SCRIPT_HPP
