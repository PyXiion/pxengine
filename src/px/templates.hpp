// Copyright (c) 2023.

//
// Created by pyxiion on 04.11.23.
//

#ifndef ENGINE_TEMPLATES_HPP
#define ENGINE_TEMPLATES_HPP
#include <type_traits>
#include <functional>
#include <experimental/type_traits>

namespace px {
  /// https://stackoverflow.com/questions/34672441/stdis-base-of-for-template-classes
  template<template <class...> class base, class derived>
  struct is_base_of_template_impl {
    template<class... Ts>
    static constexpr std::true_type test(const base<Ts...> *);

    static constexpr std::false_type test(...);

    using type = decltype(test(std::declval<derived *>()));
  };

  template<template <class...> class base, class derived>
  using is_base_of_template = typename is_base_of_template_impl<base, derived>::type;

  template<template <class...> class base, class derived>
  constexpr bool is_base_of_template_v = is_base_of_template<base, derived>::value;

  template<typename>
  struct function_traits;

  template<typename T>
  struct function_traits : public function_traits<decltype(&std::remove_reference_t<T>::operator())> {
  };

  template<typename T>
  struct function_traits<std::function<T> > : public function_traits<T> {
  };

  template<typename Return, typename... Args>
  struct function_traits<Return (*)(Args...)> : public function_traits<Return (Args...)> {
  };

  template<typename Return, typename... Args>
  struct function_traits<Return (Args...) const> : public function_traits<Return (Args...)> {
  };

  template<typename TClass, typename Return, typename... Args>
  struct function_traits<Return (TClass::*)(Args...)> : public function_traits<Return (Args...)> {
    typedef TClass instance_type;

    constexpr static bool is_member = true; // override
  };

  template<typename TClass, typename Return, typename... Args>
  struct function_traits<Return (TClass::* const)(Args...)> : public function_traits<Return (TClass::*)(Args...)> {
  };

  template<typename TClass, typename Return, typename... Args>
  struct function_traits<Return (TClass::*)(Args...) const> : public function_traits<Return (TClass::*)(Args...)> {
    constexpr static bool is_const = true; // override
  };

  template<typename TClass, typename Return, typename... Args>
  struct function_traits<Return (TClass::*
        const)(Args...) const> : public function_traits<Return (TClass::*)(Args...) const> {
  };

  template<typename Return, typename... Args>
  struct function_traits<Return (Args...)> {
    typedef Return (*signature)(Args...);

    typedef Return return_type;

    typedef std::tuple<Args...> arguments;

    static constexpr size_t argument_count = sizeof...(Args);

    constexpr static bool is_member = false;
    constexpr static bool is_const = false;
  };

  template<typename T>
  concept MemberFunction = function_traits<T>::is_member;

  template<class>
  struct field_traits;

  template<class TClass, class T>
  struct field_traits<T (TClass::*)> {
    typedef TClass instance_type;
    typedef T type;

    inline constexpr static bool is_const = false;
  };

  template<class TClass, class T>
  struct field_traits<T const (TClass::*)> {
    typedef TClass instance_type;
    typedef const T type;

    inline constexpr static bool is_const = true;
  };

  // Function signature concept
  namespace priv {
    // Function signature
    template<class T>
    struct function_signature_impl : std::false_type {
    };

    template<class T, class... TArgs>
    struct function_signature_impl<T (TArgs...)> : std::true_type {
    };

    template<class T, class... TArgs>
    struct function_signature_impl<T (TArgs...) noexcept> : std::true_type {
    };

    // Function signature with smth
    template<class T, class... WithArgs>
    struct function_signature_with_impl : std::false_type {
    };

    template<class T, class... WithArgs>
    struct function_signature_with_impl<T (WithArgs...), WithArgs...> : std::true_type {
    };

    template<class T, class... WithArgs>
    struct function_signature_with_impl<T (WithArgs...) noexcept, WithArgs...> : std::true_type {
    };

    template<class T, class... TArgs, class... WithArgs>
    struct function_signature_with_impl<T (WithArgs..., TArgs...), WithArgs...> : std::true_type {
    };

    template<class T, class... TArgs, class... WithArgs>
    struct function_signature_with_impl<T (WithArgs..., TArgs...) noexcept, WithArgs...> : std::true_type {
    };

    // Method Pointer Type
    template<class T>
    struct method_pointer_type_impl : std::false_type {
    };

    template<class Base, class T, class... TArgs>
    struct method_pointer_type_impl<T (Base::*)(TArgs...)> : std::true_type {
    };

    template<class Base, class T, class... TArgs>
    struct method_pointer_type_impl<T (Base::*)(TArgs...) noexcept> : std::true_type {
    };

    template<class Base, class T, class... TArgs>
    struct method_pointer_type_impl<T (Base::*)(TArgs...) const> : std::true_type {
    };

    template<class Base, class T, class... TArgs>
    struct method_pointer_type_impl<T (Base::*)(TArgs...) const noexcept> : std::true_type {
    };

    // Property Pointer Type
    template<class T>
    struct property_pointer_type_impl : std::false_type {
    };

    template<class Base, class T>
    struct property_pointer_type_impl<T (Base::*)> : std::true_type {
    };

    template<class Base, class T>
    struct property_pointer_type_impl<T const (Base::*)> : std::true_type {
    };
  }

  template<class T>
  concept FunctionSignature = priv::function_signature_impl<T>::value;

  template<class T, class... TArgs>
  concept FunctionSignatureWith = priv::function_signature_with_impl<T, TArgs...>::value;

  template<class T>
  concept MethodPtrType = priv::method_pointer_type_impl<T>::value;

  template<class PtrType, class Base>
  concept MethodPtrTypeOf = MethodPtrType<PtrType>
                            and std::derived_from<Base, typename function_traits<PtrType>::instance_type>;

  template<class T>
  concept PropertyPtrType = priv::property_pointer_type_impl<T>::value;

  template<class PtrType, class Base>
  concept PropertyPtrTypeOf = PropertyPtrType<PtrType>
                              and std::derived_from<Base, typename field_traits<PtrType>::instance_type>;

  template<PropertyPtrType T>
  std::ptrdiff_t getPropertyOffset(T property) {
    typedef typename field_traits<T>::instance_type Class;
    return reinterpret_cast<size_t>(reinterpret_cast<const void*>(&(reinterpret_cast<Class *>(reinterpret_cast<void*>(100000))->*property))) - 100000;
  }

  // Lord, save the StackOverflow
  // Useful links:
  // https://stackoverflow.com/questions/14336813/static-cast-in-a-template
  // https://stackoverflow.com/questions/25871381/c-overloaded-function-as-template-argument
  // https://stackoverflow.com/questions/9831501/how-can-i-have-multiple-parameter-packs-in-a-variadic-template
  // https://stackoverflow.com/questions/22291737/why-cant-decltype-work-with-overloaded-functions

  // https://www.reddit.com/r/cpp_questions/comments/pumi9r/does_c20_not_support_string_literals_as_template/
  template<auto N>
  struct string_literal {
    constexpr string_literal() = default;

    constexpr string_literal(const char (&str)[N]) {
      std::copy_n(str, N, value);
    }

    char value[N]{};
  };

  template<auto N, auto M>
  auto operator+(const string_literal<N> &l, const string_literal<M> &r) {
    string_literal<N + M> result;

    // copy left
    std::copy_n(l.value, 0 + N, result.value);
    // copy right
    std::copy_n(r.value, result.value + N, result.value);

    return result;
  }

  template<class T, class Base>
  concept SmartPointerTypeFor = requires (T ptr) {
    { ptr.get() } -> std::convertible_to<const Base*>;
    { *ptr } -> std::convertible_to<const Base&>;
    { ptr.operator->() } -> std::convertible_to<const Base*>;
    { ptr.operator bool() };
  };
}

#endif //ENGINE_TEMPLATES_HPP
