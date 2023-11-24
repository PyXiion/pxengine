// Copyright (c) 2023.

//
// Created by pyxiion on 04.11.23.
//

#ifndef ENGINE_TEMPLATES_HPP
#define ENGINE_TEMPLATES_HPP
#include <type_traits>
#include <experimental/type_traits>

namespace px {
  /// https://stackoverflow.com/questions/34672441/stdis-base-of-for-template-classes
  template <template <class...> class base, class derived>
  struct is_base_of_template_impl {
    template<class ...Ts>
    static constexpr std::true_type  test(const base<Ts...> *);
    static constexpr std::false_type test(...);

    using type = decltype(test(std::declval<derived*>()));
  };

  template <template <class...> class base, class derived>
  using is_base_of_template = typename is_base_of_template_impl<base,derived>::type;

  template <template <class...> class base, class derived>
  constexpr bool is_base_of_template_v = is_base_of_template<base,derived>::value;

  template <typename> struct function_traits;

  template <typename T>
  struct function_traits : public function_traits<decltype(&std::remove_reference_t<T>::operator())> { };

  template <typename T>
  struct function_traits<std::function<T>> : public function_traits<T> { };

  template <typename Return, typename ...Args>
  struct function_traits<Return (*)(Args...)> : function_traits<Return (Args...)> { };

  template <typename Return, typename ...Args>
  struct function_traits<Return (Args...) const> : function_traits<Return (Args...)> { };

  template <typename TClass, typename Return, typename ...Args>
  struct function_traits<Return (TClass::*)(Args...)> : function_traits<Return (Args...)> {
    typedef TClass instance_type;

    constexpr static bool is_member = true; // override
  };

  template <typename TClass, typename Return, typename ...Args>
  struct function_traits<Return (TClass::*)(Args...) const> : function_traits<Return (TClass::*)(Args...)> { };

  template <typename Return, typename ...Args>
  struct function_traits<Return (Args...)> {
    typedef Return (*signature)(Args...);
    typedef Return return_type;

    typedef std::tuple<Args...> arguments;

    static constexpr size_t argument_count = sizeof...(Args);

    constexpr static bool is_member = false;
  };

  template<typename T>
  concept MemberFunction = function_traits<T>::is_member;

  template<typename, typename...>
  struct overloaded_function_traits;

  template<typename T, typename Return, typename ...Args>
  struct overloaded_function_traits<T, Return, std::tuple<Args...>>
    : public function_traits<decltype(static_cast<Return(*)(Args...)>(std::declval<T>()))> {};

  template<MemberFunction T, typename Return, typename ...Args>
  struct overloaded_function_traits<T, Return, std::tuple<Args...>>
  : public function_traits<decltype(static_cast<Return(function_traits<T>::instance_type::*)(Args...)>(std::declval<T>()))> {};

  // Lord, save the StackOverflow
  // Useful links:
  // https://stackoverflow.com/questions/14336813/static-cast-in-a-template
  // https://stackoverflow.com/questions/25871381/c-overloaded-function-as-template-argument
  // https://stackoverflow.com/questions/9831501/how-can-i-have-multiple-parameter-packs-in-a-variadic-template
  // https://stackoverflow.com/questions/22291737/why-cant-decltype-work-with-overloaded-functions


}

#endif //ENGINE_TEMPLATES_HPP
