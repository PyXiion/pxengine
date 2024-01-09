// Copyright (c) 2023.

//
// Created by pyxiion on 14.12.23.
//

#include "object_type_builder.hpp"
#include <angelscript.h>


namespace px::script {
  void priv::registerClassType(asIScriptEngine *engine, const std::string &name, int size,
                               script::priv::ObjTypeFlags::Enum flags) {
    int r = engine->RegisterObjectType(name.c_str(), size, flags);
    if (r < 0) {
      PX_THROW_AND_LOG("AngelScript", RegisterClassTypeError, "Failed to register class type ({}, {}, {}, {})",
                       fmt::ptr(engine), name, size, flags);
    }
  }

  void priv::registerClassBehaviour(asIScriptEngine *engine, const std::string &name,
                                    priv::ObjBehaviour::Enum behaviour, const std::string &declaration, void *funcPtr) {
    int r = engine->RegisterObjectBehaviour(name.c_str(), static_cast<asEBehaviours>(behaviour), declaration.c_str(),
                                            asFUNCTION(funcPtr), asCALL_CDECL_OBJFIRST);
    if (r < 0) {
      PX_THROW_AND_LOG("AngelScript", RegisterClassBehaviourError, "Failed to register class behaviour ({}, {}, {}, {})",
                       fmt::ptr(engine), name, declaration, fmt::ptr(funcPtr));
    }
  }

  void priv::registerMethod(asIScriptEngine *engine, const std::string &name, const std::string &declaration,
                            DummyMethod funcPtr) {
    int r = engine->RegisterObjectMethod(name.c_str(), declaration.c_str(),
                                         asSMethodPtr<sizeof(DummyMethod)>::Convert(funcPtr), asCALL_THISCALL);
    if (r < 0) {
      PX_THROW_AND_LOG("AngelScript", RegisterClassMethodError, "Failed to register method ({}, {}, {})",
                       fmt::ptr(engine), name, declaration);
    }
  }

  void priv::registerProxyMethod(asIScriptEngine *engine, const std::string &name, const std::string &declaration,
                                 void *funcPtr) {
    int r = engine->RegisterObjectMethod(name.c_str(), declaration.c_str(), asFUNCTION(funcPtr), asCALL_CDECL_OBJFIRST);
    if (r < 0) {
      PX_THROW_AND_LOG("AngelScript", RegisterClassProxyMethodError, "Failed to register proxy method ({}, {}, {}, {})",
                       fmt::ptr(engine), name, declaration, fmt::ptr(funcPtr));
    }
  }

  void priv::registerCdeclMethod(asIScriptEngine *engine, const std::string &name, const std::string &declaration,
                                 void *funcPtr) {
    int r = engine->RegisterObjectMethod(name.c_str(), declaration.c_str(), asFUNCTION(funcPtr), asCALL_CDECL);
    if (r < 0) {
      PX_THROW_AND_LOG("AngelScript", RegisterClassCdeclMethodError, "Failed to register cdecl method ({}, {}, {}, {})",
                       fmt::ptr(engine), name, declaration, fmt::ptr(funcPtr));
    }
  }

  void priv::registerGenericMethod(asIScriptEngine *engine, const std::string &name, const std::string &declaration,
                                   void *funcPtr, void *auxiliary) {
    auto *function = reinterpret_cast<asGENFUNC_t>(funcPtr);
    int r = engine->RegisterObjectMethod(name.c_str(), declaration.c_str(), asFUNCTION(function), asCALL_GENERIC,
                                         auxiliary);
    if (r < 0) {
      PX_THROW_AND_LOG("AngelScript", RegisterClassGenericMethodError, "Failed to register generic method ({}, {}, {}, {})",
                       fmt::ptr(engine), name, declaration, fmt::ptr(funcPtr));
    }
  }

  void priv::registerProperty(asIScriptEngine *engine, const std::string &name, const std::string &declaration,
                              int offset) {
    int r = engine->RegisterObjectProperty(name.c_str(), declaration.c_str(), offset);
    if (r < 0) {
      PX_THROW_AND_LOG("AngelScript", RegisterClassPropertyError, "Failed to register property ({}, {}, {}, {})",
                       fmt::ptr(engine), name, declaration, offset);
    }
  }

  void priv::setCtxException(const char *msg) {
    asGetActiveContext()->SetException(msg);
  }
} // px::script
