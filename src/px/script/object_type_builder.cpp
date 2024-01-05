// Copyright (c) 2023.

//
// Created by pyxiion on 14.12.23.
//

#include "object_type_builder.hpp"
#include <angelscript.h>
#include <cassert>

namespace px::script {
  void priv::registerClassType(asIScriptEngine *engine, const std::string &name, int size,
                               script::priv::ObjTypeFlags::Enum flags) {
    int r = engine->RegisterObjectType(name.c_str(), size, flags);
    assert(r >= 0);
  }

  void priv::registerClassBehaviour(asIScriptEngine *engine, const std::string &name,
                                    priv::ObjBehaviour::Enum behaviour, const std::string &declaration, void *funcPtr) {
    int r = engine->RegisterObjectBehaviour(name.c_str(), static_cast<asEBehaviours>(behaviour), declaration.c_str(),
                                            asFUNCTION(funcPtr), asCALL_CDECL_OBJFIRST);
    assert(r >= 0);
  }

  void priv::registerMethod(asIScriptEngine *engine, const std::string &name, const std::string &declaration,
                            DummyMethod funcPtr) {
    int r = engine->RegisterObjectMethod(name.c_str(), declaration.c_str(),
                                         asSMethodPtr<sizeof(DummyMethod)>::Convert(funcPtr), asCALL_THISCALL);
    assert(r >= 0);
  }

  void priv::registerProxyMethod(asIScriptEngine *engine, const std::string &name, const std::string &declaration,
                                 void *funcPtr) {
    int r = engine->RegisterObjectMethod(name.c_str(), declaration.c_str(), asFUNCTION(funcPtr), asCALL_CDECL_OBJFIRST);
    assert(r >= 0);
  }

  void priv::registerCdeclMethod(asIScriptEngine *engine, const std::string &name, const std::string &declaration,
                                 void *funcPtr) {
    int r = engine->RegisterObjectMethod(name.c_str(), declaration.c_str(), asFUNCTION(funcPtr), asCALL_CDECL);
    assert(r >= 0);
  }

  void priv::registerGenericMethod(asIScriptEngine *engine, const std::string &name, const std::string &declaration,
  void *funcPtr, void *auxiliary) {
    auto *function = reinterpret_cast<asGENFUNC_t>(funcPtr);
    int r = engine->RegisterObjectMethod(name.c_str(), declaration.c_str(), asFUNCTION(function), asCALL_GENERIC, auxiliary);
    assert(r >= 0);
  }

  void priv::registerProperty(asIScriptEngine *engine, const std::string &name, const std::string &declaration,
                              int offset) {
    int r = engine->RegisterObjectProperty(name.c_str(), declaration.c_str(), offset);
    assert(r >= 0);
  }

  void priv::setCtxException(const char *msg) {
    asGetActiveContext()->SetException(msg);
  }
} // px::script
