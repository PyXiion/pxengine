// Copyright (c) 2023.

//
// Created by pyxiion on 13.12.23.
//
#include "function.hpp"
#include <angelscript.h>

namespace px::script {
  priv::FunctionHandle::FunctionHandle(asIScriptContext *context, asIScriptFunction *function, asIScriptObject *self)
      : m_ctx(context)
      , m_func(function)
      , m_self(self) {
    if (m_ctx)  m_ctx->AddRef();
    if (m_func) m_func->AddRef();
    if (m_self) m_self->AddRef();
  }

  priv::FunctionHandle::~FunctionHandle() {
    if (m_self) m_self->Release();
    if (m_func) m_func->Release();
    if (m_ctx) m_ctx->Release();
  }

  priv::FunctionHandle::FunctionHandle(const priv::FunctionHandle &other)
      : FunctionHandle(other.m_ctx, other.m_func) {
  }

  priv::FunctionHandle &priv::FunctionHandle::operator=(const priv::FunctionHandle &other) {
    if (&other != this) {
      m_self = other.m_self;
      m_func = other.m_func;
      m_ctx = other.m_ctx;
      if (m_self) m_self->AddRef();
      if (m_ctx) m_ctx->AddRef();
      if (m_func) m_func->AddRef();
    }
    return *this;
  }

  priv::FunctionHandle::FunctionHandle(priv::FunctionHandle &&other) noexcept
      : m_ctx(other.m_ctx)
      , m_func(other.m_func)
      , m_self(other.m_self) {
    other.m_ctx = nullptr;
    other.m_func = nullptr;
    other.m_self = nullptr;
  }

  priv::FunctionHandle &priv::FunctionHandle::operator=(priv::FunctionHandle &&other) noexcept {
    if (&other != this) {
      m_ctx = other.m_ctx;
      m_func = other.m_func;
      other.m_ctx = nullptr;
      other.m_func = nullptr;
    }
    return *this;
  }

  void priv::FunctionHandle::setThis(asIScriptObject *self) {
    if (m_self)
      m_self->Release();
    m_self = self;
    if (m_self)
      m_self->AddRef();
  }

  void priv::FunctionHandle::prepare() {
    m_ctx->Prepare(m_func);
    CVLOG(2, "AngelScript") << "Preparing context " << m_ctx << " with function " << m_func;

    if (m_self) {
      CVLOG(2, "AngelScript") << "\tSelf: " << m_self;
      m_ctx->SetObject(m_self);
    }
  }

  void priv::FunctionHandle::setArg(int arg, std::int8_t value) {
    CVLOG(2, "AngelScript") << "\tArg[" << arg << "] int8 " << value << " (" << static_cast<std::uint8_t>(value) << ")";
    m_ctx->SetArgByte(arg, value);
  }

  void priv::FunctionHandle::setArg(int arg, std::int16_t value) {
    CVLOG(2, "AngelScript") << "\tArg[" << arg << "] int16 " << value << " (" << static_cast<std::uint16_t>(value) << ")";
    m_ctx->SetArgWord(arg, value);
  }

  void priv::FunctionHandle::setArg(int arg, std::int32_t value) {
    CVLOG(2, "AngelScript") << "\tArg[" << arg << "] int32 " << value << " (" << static_cast<std::uint32_t>(value) << ")";
    m_ctx->SetArgDWord(arg, value);
  }

  void priv::FunctionHandle::setArg(int arg, std::int64_t value) {
    CVLOG(2, "AngelScript") << "\tArg[" << arg << "] int64 " << value << " (" << static_cast<std::uint64_t>(value) << ")";
    m_ctx->SetArgQWord(arg, value);
  }

  void priv::FunctionHandle::setArg(int arg, float value) {
    CVLOG(2, "AngelScript") << "\tArg[" << arg << "] float " << value;
    m_ctx->SetArgFloat(arg, value);
  }

  void priv::FunctionHandle::setArg(int arg, double value) {
    CVLOG(2, "AngelScript") << "\tArg[" << arg << "] double " << value;
    m_ctx->SetArgDouble(arg, value);
  }

  void priv::FunctionHandle::setArgAddress(int arg, void **ptr) {
    CVLOG(2, "AngelScript") << "\tArg[" << arg << "] address " << ptr;
    m_ctx->SetArgAddress(arg, ptr);
  }

  void priv::FunctionHandle::setArgObject(int arg, void *ptr) {
    CVLOG(2, "AngelScript") << "\tArg[" << arg << "] object " << ptr;
    m_ctx->SetArgObject(arg, ptr);
  }

  int priv::FunctionHandle::execute() {
    CVLOG(2, "AngelScript") << "Executing context " << m_ctx;
    return m_ctx->Execute();
  }

  template<>
  std::int8_t priv::FunctionHandle::getReturn() {
//    CVLOG(2, "AngelScript") << "\tGot byte return " << m_ctx;
    return m_ctx->GetReturnByte();
  }

  template<>
  std::int16_t priv::FunctionHandle::getReturn() {
    return m_ctx->GetReturnWord();
  }

  template<>
  std::int32_t priv::FunctionHandle::getReturn() {
    return m_ctx->GetReturnDWord();
  }

  template<>
  std::int64_t priv::FunctionHandle::getReturn() {
    return m_ctx->GetReturnQWord();
  }

  template<>
  float priv::FunctionHandle::getReturn() {
    return m_ctx->GetReturnFloat();
  }

  template<>
  double priv::FunctionHandle::getReturn() {
    return m_ctx->GetReturnDouble();
  }

  void *priv::FunctionHandle::getReturnAddress() {
    return m_ctx->GetAddressOfReturnValue();
  }

  void *priv::FunctionHandle::getReturnObject() {
    return m_ctx->GetReturnObject();
  }

  const char *priv::FunctionHandle::getDeclaration() {
    return m_func->GetDeclaration();
  }

} // px::script