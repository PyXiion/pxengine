// Copyright (c) 2023.

//
// Created by pyxiion on 14.12.23.
//

#include "object_type_builder.hpp"
#include <angelscript.h>

namespace px::script {
  void priv::registerClassType(asIScriptEngine *engine, const std::string &name, int size,
                                  script::priv::ObjTypeFlags::Enum flags) {
    engine->RegisterObjectType(name.c_str(), size, flags);
  }

  void priv::registerClassBehaviour(asIScriptEngine *engine, const std::string &name,
                                  priv::ObjBehaviour::Enum behaviour, const std::string &declaration, void *funcPtr) {
    engine->RegisterObjectBehaviour(name.c_str(), static_cast<asEBehaviours>(behaviour), declaration.c_str(), asFUNCTION(funcPtr), asCALL_CDECL_OBJFIRST);
  }

  void priv::registerMethod(asIScriptEngine *engine, const std::string &name, const std::string &declaration, DummyMethod funcPtr) {
    engine->RegisterObjectMethod(name.c_str(), declaration.c_str(), asSMethodPtr<sizeof(DummyMethod)>::Convert(funcPtr), asCALL_THISCALL);
  }

  void priv::registerProxyMethod(asIScriptEngine *engine, const std::string &name, const std::string &declaration, void *funcPtr) {
    engine->RegisterObjectMethod(name.c_str(), declaration.c_str(), asFUNCTION(funcPtr), asCALL_CDECL_OBJFIRST);
  }

  void priv::registerProperty(asIScriptEngine *engine, const std::string &name, const std::string &declaration, int offset) {
    engine->RegisterObjectProperty(name.c_str(), declaration.c_str(), offset);
  }

  extern void priv::setCtxException(const char *msg) {
    asGetActiveContext()->SetException(msg);
  }
} // px::script