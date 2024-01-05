// Copyright (c) 2024.

//
// Created by pyxiion on 05.01.24.
//
#include "common.hpp"

#include <angelscript.h>

namespace px::script {
  GenericManipulator::GenericManipulator(asIScriptGeneric *generic)
    : m_generic(generic) {}

  void *GenericManipulator::getAuxiliary() const {
    return m_generic->GetAuxiliary();
  }

  void *GenericManipulator::getObject() const {
    return m_generic->GetObject();
  }

  void GenericManipulator::setReturnAddress(void *ptr) {
    m_generic->SetReturnAddress(ptr);
  }

  void *GenericManipulator::getArgAddress(unsigned index) {
    return m_generic->GetArgAddress(index);
  }

  template<>
  int8_t GenericManipulator::getArg(const unsigned index) {
    return m_generic->GetArgByte(index);
  }

  template<>
  int16_t GenericManipulator::getArg(const unsigned index) {
    return m_generic->GetArgWord(index);
  }

  template<>
  int32_t GenericManipulator::getArg(const unsigned index) {
    return m_generic->GetArgDWord(index);
  }

  template<>
  int64_t GenericManipulator::getArg(const unsigned index) {
    return m_generic->GetArgQWord(index);
  }

  template<>
  float GenericManipulator::getArg(const unsigned index) {
    return m_generic->GetArgFloat(index);
  }

  template<>
  double GenericManipulator::getArg(const unsigned index) {
    return m_generic->GetArgDouble(index);
  }
}
