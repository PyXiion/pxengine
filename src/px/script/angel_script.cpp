// Copyright (c) 2023.

//
// Created by pyxiion on 11.12.23.
//

#include "angel_script.hpp"
#include <angelscript.h>
#include <mutex>
#include <cassert>
#include "addons/scriptarray/scriptarray.h"
#include "addons/scriptstdstring/scriptstdstring.h"

namespace px::script {

  static std::once_flag createLoggerFlag;
  static void createLogger() {
    auto logger = el::Loggers::getLogger("AngelScript");

    auto conf = *logger->configurations();
    conf.set(el::Level::Global,
              el::ConfigurationType::Format,
              "%user [%datetime] [%level] \t%logger : %msg");
    logger->configure(conf);
  }

  AngelScript::AngelScript() {
    std::call_once(createLoggerFlag, createLogger);

    m_handle = asCreateScriptEngine();
    CLOG(INFO, "AngelScript") << "Created the AngelScript engine";

    init();
  }

  AngelScript::~AngelScript() {
    if (m_ctx)
      m_ctx->Release();

    m_handle->ShutDownAndRelease();
    CLOG(INFO, "AngelScript") << "Destroyed the AngelScript";
  }

  void AngelScript::init() {
    m_handle->SetMessageCallback(asFUNCTION(&AngelScript::messageCallback), this, asCALL_CDECL);

    RegisterScriptArray(m_handle, true);

    RegisterStdString(m_handle);
    RegisterStdStringUtils(m_handle);
    CLOG(INFO, "AngelScript") << "Registered AngelScript addons array, string, string utils";

    m_ctx = m_handle->CreateContext();
    CLOG(INFO, "AngelScript") << "Created the AngelScript context.";
  }

  void AngelScript::registerGlobalFunction(const std::string &funSign, void *funPtr) {
    assert(m_handle->RegisterGlobalFunction(funSign.c_str(), asFUNCTION(funPtr), asCALL_CDECL) >= 0);
  }

  void AngelScript::registerGlobalVariable(const std::string &signature, void *ptr) {
    assert(m_handle->RegisterGlobalProperty(signature.c_str(), ptr) >= 0);
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

    auto message = fmt::format("{} ({}, {}) : {}", msg->section, msg->row, msg->col, msg->message);

    switch (msg->type) {
      default:
      case asMSGTYPE_INFORMATION:
        CLOG(INFO, "AngelScript") << message;
        break;
      case asMSGTYPE_WARNING:
        CLOG(WARNING, "AngelScript") << message;
        break;
      case asMSGTYPE_ERROR:
        CLOG(ERROR, "AngelScript") << message;
    }
  }
} // px::script