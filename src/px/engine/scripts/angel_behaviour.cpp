// Copyright (c) 2023.

//
// Created by pyxiion on 26.12.23.
//

#include "angel_behaviour.hpp"
#include "px/script/angel_script.hpp"
#include <angelscript.h>
#include <cassert>


namespace px {
  AngelBehaviour::AngelBehaviour(asIScriptObject *object)
    : m_obj(object)
    , m_isDead(nullptr) {
    m_isDead = object->GetWeakRefFlag();
    (void) m_isDead->AddRef();
  }

  AngelBehaviour::~AngelBehaviour() {
    (void) m_isDead->Release();
  }

  void AngelBehaviour::bindToEngine(script::AngelScript &as) {
    auto *engine = as.getHandle();
    int r = engine->RegisterObjectType("AngelBehaviour_t", 0, asOBJ_REF | asOBJ_NOHANDLE); assert(r >= 0);
  }

  std::shared_ptr<AngelBehaviour> AngelBehaviour::factory() {
    asIScriptContext *ctx = asGetActiveContext();

    // Get the function that is calling the factory so we can be certain it is the AngelScript script class
    const asIScriptFunction *func = ctx->GetFunction(0);
    const asITypeInfo *info       = func->GetObjectType();

    if (info == nullptr or std::string(info->GetName()) != "AngelBehaviour") {
      ctx->SetException("Invalid attempt to manually instantiate AngelBehaviour_t");
      return nullptr;
    }

    auto *obj = reinterpret_cast<asIScriptObject *>(ctx->GetThisPointer());

    return std::make_shared<AngelBehaviour>(obj);
  }
} // px