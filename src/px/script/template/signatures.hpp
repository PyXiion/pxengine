// Copyright (c) 2023.

//
// Created by pyxiion on 12.12.23.
//

#ifndef PX_ENGINE_SIGNATURES_HPP
#define PX_ENGINE_SIGNATURES_HPP
#include <tuple>
#include <ranges>
#include <string_view>
#include <fmt/format.h>
#include "names.hpp"
#include "px/templates.hpp"

namespace px::script {
  namespace priv {
    template<class...>
    struct getArrOfNamesImpl;

    template<class ...T>
    struct getArrOfNamesImpl<std::tuple<T...>> {
      static auto constexpr inline N = sizeof...(T);

      static inline decltype(auto) get() {
        std::array<std::string_view, N> arr {
          getTypeAsName<T, true>()...
        };
        return arr;
      }
    };

    template<class>
    struct TupleFirstElem;

    template<class TFirst, class ...TOther>
    struct TupleFirstElem<std::tuple<TFirst, TOther...>> {
      typedef TFirst First;
      typedef std::tuple<TOther...> Other;
    };
  }

  template<class T>
  static inline decltype(auto) getSignature(std::string_view &&functionName) {
    typedef function_traits<T> Traits;
    auto &&retType = getTypeAsName<typename Traits::return_type>();

    typedef typename Traits::arguments Arguments;
    auto &&args = priv::getArrOfNamesImpl<Arguments>::get();

    std::string_view postfix = "";
    if constexpr (Traits::is_const) {
      postfix = " const";
    }

    return fmt::format("{0} {1}({2}){3}",
                       std::forward<std::string_view>(retType),
                       std::forward<std::string_view>(functionName),
                       fmt::join((args), ", "),
                       postfix);
  }

  template<class T>
  static inline decltype(auto) getProxySignature(std::string_view &&functionName, bool isConst) {
    typedef function_traits<T> Traits;
    static_assert(not Traits::is_member);

    auto &&retType = getTypeAsName<typename Traits::return_type>();

    typedef typename Traits::arguments Arguments;
    typedef priv::TupleFirstElem<Arguments> SplittedArgs;
    static_assert(std::is_pointer_v<typename SplittedArgs::First>);

    auto &&args = priv::getArrOfNamesImpl<typename SplittedArgs::Other>::get();

    std::string_view postfix = isConst ? " const" : "";

    return fmt::format("{0} {1}({2}){3}",
                       std::forward<std::string_view>(retType),
                       std::forward<std::string_view>(functionName),
                       fmt::join((args), ", "),
                       postfix);
  }

  template<class T>
  static inline decltype(auto) getPropertySignature(std::string_view &&propertyName) {
    typedef px::field_traits<T> Traits;
    decltype(auto) type = getTypeAsName<typename Traits::type>();

    if constexpr (Traits::is_const) {
      return fmt::format("const {} {}", std::forward<decltype(type)>(type), std::forward<decltype(propertyName)>(propertyName));
    } else {
      return fmt::format("{} {}", std::forward<decltype(type)>(type), std::forward<decltype(propertyName)>(propertyName));
    }
  }
} // px::script

#endif //PX_ENGINE_SIGNATURES_HPP
