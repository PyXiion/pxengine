// Copyright (c) 2023.

//
// Created by pyxiion on 11.12.23.
//

#include "angel_script.hpp"
#include <angelscript.h>
#include "addons/scriptarray/scriptarray.h"
#include "addons/scriptstdstring/scriptstdstring.h"

namespace px::script {
  AngelScript::AngelScript() {
    m_handle = asCreateScriptEngine();

    init();
  }

  AngelScript::~AngelScript() {
    if (m_ctx)
      m_ctx->Release();

    m_handle->ShutDownAndRelease();
  }

  void AngelScript::init() {
    m_handle->SetMessageCallback(asFUNCTION(&AngelScript::messageCallback), this, asCALL_CDECL);

    RegisterScriptArray(m_handle, true);

    RegisterStdString(m_handle);
    RegisterStdStringUtils(m_handle);

    m_ctx = m_handle->CreateContext();
  }

  void AngelScript::registerGlobalFunction(const std::string &funSign, void *funPtr) {
    m_handle->RegisterGlobalFunction(funSign.c_str(), asFUNCTION(funPtr), asCALL_CDECL);
  }

  asIScriptEngine *AngelScript::getHandle() {
    return m_handle;
  }

  asIScriptContext *AngelScript::getContext() {
    return m_ctx;
  }

  ModuleBuilder AngelScript::createModuleBuilder() {
    return ModuleBuilder(*this);
  }

  Module AngelScript::getExistingModule(const std::string &moduleName) {
    return Module(getContext(), m_handle->GetModule(moduleName.c_str()));
  }

  void AngelScript::messageCallback(const asSMessageInfo *msg, void *param) {
    auto self = reinterpret_cast<AngelScript*>(param);
    const char *msgTypes[] = {"ERROR", "WARNING", "INFO"};

    fmt::print("{} ({}, {}) : {} : {}\n", msg->section, msg->row, msg->col, msgTypes[(int)msg->type], msg->message);
  }
} // px::script