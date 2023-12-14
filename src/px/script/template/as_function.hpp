// Copyright (c) 2023.

//
// Created by pyxiion on 12.12.23.
//

#ifndef PX_ENGINE_AS_FUNCTION_HPP
#define PX_ENGINE_AS_FUNCTION_HPP
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

      static inline auto get() {
        std::array<std::string_view, N> arr {
          getTypeAsName<T>()...
        };
        return arr;
      }
    };
  }

  template<class function_traits>
  static inline std::string getSignature(std::string_view functionName) {
    auto retType = getTypeAsName<typename function_traits::return_type>();

    typedef typename function_traits::arguments Arguments;
    auto args = priv::getArrOfNamesImpl<Arguments>::get();

    return fmt::format("{0} {1}({2})", retType, functionName, fmt::join(args, ", "));
  }

  template<class, class...>
  class AsFunction;

  template<class TReturn, class ...TArgs>
  class AsFunction<TReturn (TArgs...)> {
  public:
    typedef px::function_traits<TReturn (TArgs...)> FunctionTraits;
    typedef TReturn (*FunctionPtr)(TArgs...);

  public:
    explicit AsFunction(FunctionPtr &&ptr)
      : m_ptr(ptr) {}

    inline static std::string getSignature(std::string_view functionName) {
      return px::script::getSignature<FunctionTraits>(functionName);
    }
  private:
    FunctionPtr m_ptr;
  };

  template<class TReturn, class ...TArgs>
  class AsFunction<px::function_traits<TReturn (TArgs...)>> : AsFunction<TReturn (TArgs...)> {};

  template<class TReturn, class ...TArgs>
  class AsFunction<TReturn, std::tuple<TArgs...>> : AsFunction<TReturn (TArgs...)> {};
} // px::script

#endif //PX_ENGINE_AS_FUNCTION_HPP
