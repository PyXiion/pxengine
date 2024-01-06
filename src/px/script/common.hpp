// Copyright (c) 2023.

//
// Created by pyxiion on 12.12.23.
//

#ifndef PX_ENGINE_COMMON_HPP
#define PX_ENGINE_COMMON_HPP
#include <angelscript.h>
#include <cstdint>
#include <type_traits>

struct asIScriptEngine;
struct asIScriptModule;
struct asIScriptFunction;
struct asIScriptObject;
struct asIScriptGeneric;

struct asIScriptContext;

struct asITypeInfo;

struct asSMessageInfo;

namespace px::script {
  template<class T>
  concept AsPrimitive = std::is_same_v<T, std::int8_t> or
                        std::is_same_v<T, std::int16_t> or
                        std::is_same_v<T, std::int32_t> or
                        std::is_same_v<T, std::int64_t> or
                        std::is_same_v<T, float> or
                        std::is_same_v<T, double>;

  template<class T>
  concept AsAddress = std::is_pointer_v<T> or std::is_reference_v<T>;

  template<class T>
  concept AsObject = not AsPrimitive<T> and not AsAddress<T>;

  class GenericManipulator {
  public:
    explicit GenericManipulator(asIScriptGeneric *generic);

    [[nodiscard]] void *getAuxiliary() const;
    [[nodiscard]] void *getObject() const;

    template<class T>
    T getArg(unsigned index);

    void setReturnAddress(void *);

  private:
    asIScriptGeneric *m_generic;

    void *getArgAddress(unsigned index);
  };

  template<class T>
  T GenericManipulator::getArg(const unsigned index) {
    using Ptr = std::remove_reference_t<T>*;
    return *reinterpret_cast<Ptr>(m_generic->GetAddressOfArg(index));
  }
}

#endif //PX_ENGINE_COMMON_HPP
