// Copyright (c) 2023.

//
// Created by pyxiion on 13.12.23.
//
#include "function.hpp"
#include <angelscript.h>

namespace px::script {
  priv::FunctionHandle::FunctionHandle(asIScriptContext *context, asIScriptFunction *function)
      : m_ctx(context)
      , m_func(function) {
    if (m_ctx) m_ctx->AddRef();
    if (m_func) m_func->AddRef();
  }

  priv::FunctionHandle::~FunctionHandle() {
    if (m_func) m_func->Release();
    if (m_ctx) m_ctx->Release();
  }

  priv::FunctionHandle::FunctionHandle(const priv::FunctionHandle &other)
      : FunctionHandle(other.m_ctx, other.m_func) {
  }

  priv::FunctionHandle &priv::FunctionHandle::operator=(const priv::FunctionHandle &other) {
    if (&other != this) {
      m_func = other.m_func;
      m_ctx = other.m_ctx;
      if (m_ctx) m_ctx->AddRef();
      if (m_func) m_func->AddRef();
    }
    return *this;
  }

  priv::FunctionHandle::FunctionHandle(priv::FunctionHandle &&other) noexcept
      : m_ctx(other.m_ctx)
      , m_func(other.m_func) {
    other.m_ctx = nullptr;
    other.m_func = nullptr;
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

  void priv::FunctionHandle::prepare() {
    m_ctx->Prepare(m_func);
  }

  void priv::FunctionHandle::setArg(int arg, std::int8_t value) {
    m_ctx->SetArgByte(arg, value);
  }

  void priv::FunctionHandle::setArg(int arg, std::int16_t value) {
    m_ctx->SetArgWord(arg, value);
  }

  void priv::FunctionHandle::setArg(int arg, std::int32_t value) {
    m_ctx->SetArgDWord(arg, value);
  }

  void priv::FunctionHandle::setArg(int arg, std::int64_t value) {
    m_ctx->SetArgQWord(arg, value);
  }

  void priv::FunctionHandle::setArg(int arg, float value) {
    m_ctx->SetArgFloat(arg, value);
  }

  void priv::FunctionHandle::setArg(int arg, double value) {
    m_ctx->SetArgDouble(arg, value);
  }

  void priv::FunctionHandle::setArgAddress(int arg, void *ptr) {
    m_ctx->SetArgAddress(arg, ptr);
  }

  void priv::FunctionHandle::setArgObject(int arg, void *ptr) {
    m_ctx->SetArgObject(arg, ptr);
  }

  int priv::FunctionHandle::execute() {
    return m_ctx->Execute();
  }

  template<>
  std::int8_t priv::FunctionHandle::getReturn() {
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