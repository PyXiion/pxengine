// Copyright (c) 2024.

//
// Created by pyxiion on 14.01.24.
//

#include "c_script_type_of.hpp"

#include <cassert>
#include <new>

CScriptTypeOf::CScriptTypeOf(int typeId)
  : typeId(typeId) {}

namespace {
  void constructor(void *memory, asITypeInfo *type) {
    new(memory) CScriptTypeOf(type->GetSubTypeId());
  }

  void destructor(void *memory) {
    // do nothing
  }

  int toInt(const CScriptTypeOf &typeOf) {
    return typeOf.typeId;
  }
}

void RegisterTypeOf(asIScriptEngine *engine) {
  int r;
  r = engine->RegisterObjectType("CTypeId", sizeof(int), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<int>()); assert(r >= 0);

  r = engine->RegisterObjectType("typeof<class T>", sizeof(CScriptTypeOf),
                             asOBJ_VALUE | asOBJ_TEMPLATE); assert(r >= 0);

  r = engine->RegisterObjectBehaviour("typeof<T>", asBEHAVE_CONSTRUCT, "void f(int &in)", asFUNCTION(constructor), asCALL_CDECL_OBJFIRST); assert(r >= 0);
  r = engine->RegisterObjectBehaviour("typeof<T>", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(destructor), asCALL_CDECL_OBJFIRST); assert(r >= 0);

  r = engine->RegisterObjectMethod("typeof<T>", "CTypeId opImplCast() const", asFUNCTION(toInt), asCALL_CDECL_OBJFIRST); assert(r >= 0);

  r = engine->RegisterObjectProperty("typeof<T>", "const int id", asOFFSET(CScriptTypeOf, typeId)); assert(r >= 0);
}
