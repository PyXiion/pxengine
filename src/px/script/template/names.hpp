// Copyright (c) 2023.

//
// Created by pyxiion on 12.12.23.
//

#ifndef PX_ENGINE_NAMES_HPP
#define PX_ENGINE_NAMES_HPP
#include <string>
#include <string_view>
#include <cstdint>
#include <px/templates.hpp>
#include "px/utils/static_warning.hpp"

namespace px::script {
  template<class T>
  std::string getTypeBaseAsName();
}
// Primitives

#define PX_AS_TYPENAME(cppType, asTypeName)                     \
  template<>                                                    \
  inline std::string px::script::getTypeBaseAsName<cppType>() {             \
    return asTypeName;                                          \
  }

#define PX_AS_TYPENAME_SMART_PTR(cppType, asTypeName)                     \
  template<>                                                              \
  inline std::string px::script::getTypeBaseAsName<std::shared_ptr<cppType>>() {      \
    return asTypeName;                                                    \
  }

PX_AS_TYPENAME(void,       "void")

PX_AS_TYPENAME(bool,       "bool")

PX_AS_TYPENAME(int8_t,     "int8")
PX_AS_TYPENAME(uint8_t,   "uint8")

PX_AS_TYPENAME(int16_t,    "int16")
PX_AS_TYPENAME(uint16_t,  "uint16")

PX_AS_TYPENAME(int32_t,    "int")
PX_AS_TYPENAME(uint32_t,  "uint")

PX_AS_TYPENAME(int64_t,    "int64")
PX_AS_TYPENAME(uint64_t,  "uint64")

PX_AS_TYPENAME(float,      "float")
PX_AS_TYPENAME(double,     "double")

PX_AS_TYPENAME(std::string,"string")

namespace px {
  namespace script {
    template<class T>
    struct in {
      typedef T Type;
    };

    template<class T>
    struct out {
      typedef T Type;
    };

    template<class T>
    struct inout {
      typedef T Type;
    };

    namespace priv {
      template<class T>
      struct GetTypeAsNameImpl {
        inline static std::string name = getTypeBaseAsName<T>();

        typedef T Type;
      };

      template<class T>
      struct GetTypeAsNameImpl<const T> {
        inline static std::string name = "const " + getTypeBaseAsName<T>();

        typedef T Type;
      };

      template<>
      struct GetTypeAsNameImpl<void> {
        inline static std::string name = getTypeBaseAsName<void>();

        typedef void Type;
      };

      template<class T>
      struct GetTypeAsNameImpl<T&> {
        inline static std::string name = getTypeBaseAsName<T>() + " &";

        typedef T& Type;
      };

      template<class T>
      struct GetTypeAsNameImpl<const T&> {
        inline static std::string name = "const " + getTypeBaseAsName<T>() + " &";

        typedef const T& Type;
      };

      template<class T>
      struct GetTypeAsNameImpl<in<T>> {
        inline static std::string name = getTypeBaseAsName<T>() + " &in";

        typedef T& Type;
      };

      template<class T>
      struct GetTypeAsNameImpl<const in<T>> {
        inline static std::string name = "const " + getTypeBaseAsName<T>() + " &in";

        typedef const T& Type;
      };

      template<class T>
      struct GetTypeAsNameImpl<out<T>> {
        inline static std::string name = getTypeBaseAsName<T>() + " &out";

        typedef T& Type;
      };

      template<class T>
      struct GetTypeAsNameImpl<inout<T>> {
        inline static std::string name = getTypeBaseAsName<T>() + " &inout";

        typedef T& Type;
      };

      template<class T>
      struct CorrectedArgReference {
        typedef T Type;
      };

      template<class T>
      struct CorrectedArgReference<const T&> {
        typedef const in<T> Type;
      };

      template<class T>
      struct CorrectedArgReference<const in<T>> {
        typedef const in<T> Type;
      };
      template<class T>
      struct CorrectedArgReference<const in<T>&> : public CorrectedArgReference<const in<T>> {};

    } // priv

    template<class T, bool arg = false>
    constexpr inline std::string_view getTypeAsName() {
      if constexpr (arg) {
        return priv::GetTypeAsNameImpl<typename priv::CorrectedArgReference<T>::Type>::name;
      } else {
        // ignore in, out, inout mods
        return priv::GetTypeAsNameImpl<typename priv::GetTypeAsNameImpl<T>::Type>::name;
      }
    }

    template<class T>
    using getType = typename priv::GetTypeAsNameImpl<T>::Type;

  } // px

  // Instead of "in", just "const Type &" should be used.
  using script::out;
  using script::inout;
} // script


#endif //PX_ENGINE_NAMES_HPP
