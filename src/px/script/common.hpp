// Copyright (c) 2023.

//
// Created by pyxiion on 12.12.23.
//

#ifndef PX_ENGINE_COMMON_HPP
#define PX_ENGINE_COMMON_HPP
#include <cstdint>
#include <type_traits>

struct asIScriptEngine;
struct asIScriptModule;
struct asIScriptFunction;
struct asIScriptObject;

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
}

#endif //PX_ENGINE_COMMON_HPP
