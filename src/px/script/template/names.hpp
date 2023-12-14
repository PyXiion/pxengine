// Copyright (c) 2023.

//
// Created by pyxiion on 12.12.23.
//

#ifndef PX_ENGINE_NAMES_HPP
#define PX_ENGINE_NAMES_HPP
#include <string>
#include <string_view>
#include <cstdint>

namespace px::script {
  template<class T>
  std::string getTypeBaseAsName();

#define PX_TYPE(cppType, asTypeName)                            \
  template<>                                                    \
  inline std::string getTypeBaseAsName<cppType>() {  \
    return asTypeName;                                                  \
  }

  PX_TYPE(void,       "void")

  PX_TYPE(int8_t,     "int8")
  PX_TYPE(uint8_t,   "uint8")

  PX_TYPE(int16_t,    "int16")
  PX_TYPE(uint16_t,  "uint16")

  PX_TYPE(int32_t,    "int")
  PX_TYPE(uint32_t,  "uint")

  PX_TYPE(int64_t,    "int64")
  PX_TYPE(uint64_t,  "uint64")

  PX_TYPE(float,      "float")
  PX_TYPE(double,     "double")

  PX_TYPE(std::string,"string")

#undef PX_TYPE


  namespace priv {
    template<class T>
    struct GetTypeAsNameImpl {
      inline static std::string name = getTypeBaseAsName<T>();
    };

    template<class T>
    struct GetTypeAsNameImpl<T&> {
      inline static std::string name = getTypeBaseAsName<T>() + " &";
    };

    template<class T>
    struct GetTypeAsNameImpl<const T> {
      inline static std::string name = "const " + getTypeBaseAsName<T>();
    };

    template<class T>
    struct GetTypeAsNameImpl<const T &> {
      inline static std::string name = "const " + getTypeBaseAsName<T>() + " &in";
    };

    template<class T>
    struct GetTypeAsNameImpl<T *> {
      inline static std::string name = getTypeBaseAsName<T>() + " @";
    };

    template<class T>
    struct GetTypeAsNameImpl<const T *> {
      inline static std::string name = "const " + getTypeBaseAsName<T>() + " @";
    };
  } // priv

  template<class T>
  constexpr inline std::string_view getTypeAsName() {
    return priv::GetTypeAsNameImpl<T>::name;
  }

} // px::script

#endif //PX_ENGINE_NAMES_HPP
