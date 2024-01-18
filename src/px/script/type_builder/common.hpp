// Copyright (c) 2023.

//
// Created by pyxiion on 14.12.23.
//

#ifndef PX_SCRIPT_TYPE_BUILDER_COMMON_HPP
#define PX_SCRIPT_TYPE_BUILDER_COMMON_HPP
#include <type_traits>
#include <new>
#include <utility>
#include "../common.hpp"
#include "px/memory/weak_ref.hpp"

namespace px::script {
  namespace priv {
    template<class T>
    struct TypeTraits {
      constexpr static inline bool hasDefaultConstructor = std::is_default_constructible_v<T>;
      constexpr static inline bool hasDestructor         = std::is_destructible_v<T>;
      constexpr static inline bool hasAssignmentOperator = requires(T &a, const T &b) { a = b; };
      constexpr static inline bool hasCopyConstructor    = std::is_copy_constructible_v<T>;

      constexpr static inline bool isFloat     = std::is_floating_point_v<T>;
      constexpr static inline bool isPrimitive = std::is_integral_v<T> or std::is_pointer_v<T> or std::is_enum_v<T>;
      constexpr static inline bool isClass     = std::is_class_v<T>;
      constexpr static inline bool isArray     = std::is_array_v<T>;

      constexpr static inline bool isAlign8 = alignof(T) == 8;

      template<class TReturn, class ...TArgs>
      using MethodType = TReturn (T::*)(TArgs ...);

      template<class U>
      using FieldType = U (T::*);
    };

    // Helpers
    template<class T, class ...TArgs>
    void Constructor(void *memory, TArgs && ...args) {
      new(memory) T(std::forward<TArgs>(args) ...);
    }

    template<class T>
    void CopyConstructor(void *memory, const T &other) {
      new(memory) T(other);
    }

    template<class T>
    void Destructor(void *memory) {
      reinterpret_cast<T *>(memory)->~T();
    }

    template<class T, class ...TArgs>
    T *Factory(TArgs && ...args) {
      return new T(std::forward<TArgs>(args) ...);
    }

    template<class T, class U = T>
    T &assigment(T &self, const U &other) {
      return self = other;
    }

    template<class T>
    T getCopy(const T other) {
      return T(other);
    }

    template<class>
    struct ValueFactorySignatureImpl : std::false_type {};

    template<class ...TArgs>
    struct ValueFactorySignatureImpl<void (void *, TArgs ...)> : std::true_type {
      using Args = std::tuple<TArgs ...>;
    };

    template<class T>
    concept ValueFactorySignature = ValueFactorySignatureImpl<T>::value;


    template<class>
    struct FactorySignatureImpl : std::false_type {};

    template<class Type, class ...TArgs>
    struct FactorySignatureImpl<Type *(TArgs ...)> : std::true_type {};

    template<class T>
    concept FactorySignature = FactorySignatureImpl<T>::value;

    template<class T, class Base>
    concept ConstProxyFunctionOf = FunctionSignatureStartsWith<T, const Base *> or FunctionSignatureStartsWith<T, const Base &>;

    template<class T, class Base>
    concept ProxyFunctionOf = FunctionSignatureStartsWith<T, Base *> or FunctionSignatureStartsWith<T, Base &> or
                              ConstProxyFunctionOf<T, Base>;

    template<class>
    struct CGetterProxyImpl : std::false_type {};

    template<class Base, class Type>
    struct CGetterProxyImpl<const Type &(Base *)> : std::true_type {};

    template<class Base, class Type>
    struct CGetterProxyImpl<const Type &(const Base *)> : std::true_type {};

    template<class Base, class Type>
    struct CGetterProxyImpl<const Type &(Base &)> : std::true_type {};

    template<class Base, class Type>
    struct CGetterProxyImpl<const Type &(const Base &)> : std::true_type {};

    template<class T>
    concept CGetterProxy = CGetterProxyImpl<T>::value;

    template<class>
    struct GetterProxyImpl : std::false_type {};

    template<class Base, class Type>
    struct GetterProxyImpl<Type &(Base *)> : std::true_type {};

    template<class Base, class Type>
    struct GetterProxyImpl<Type &(Base &)> : std::true_type {};

    template<class T>
    concept GetterProxy = GetterProxyImpl<T>::value;

    template<class>
    struct SetterProxyImpl : std::false_type {};

    template<class Base, class Type>
    struct SetterProxyImpl<void (Base *, const Type &)> : std::true_type {};

    template<class Base, class Type>
    struct SetterProxyImpl<void (Base &, const Type &)> : std::true_type {};

    template<class T>
    concept SetterProxy = SetterProxyImpl<T>::value;

    template<class T>
    struct MethodPtrToMethodProxyPtrImpl;

    template<class TClass, class TReturn, class ...TArgs>
    struct MethodPtrToMethodProxyPtrImpl<TReturn (TClass::*)(TArgs ...)> {
      using Type = TReturn (*)(TClass *, TArgs ...);
      using Handle = TClass *;
    };

    template<class TClass, class TReturn, class ...TArgs>
    struct MethodPtrToMethodProxyPtrImpl<TReturn (TClass::*)(TArgs ...) const> {
      using Type = TReturn (*)(const TClass *, TArgs ...);
      using Handle = const TClass *;
    };

    template<class TClass, class TReturn, class ...TArgs>
    struct MethodPtrToMethodProxyPtrImpl<TReturn (TClass::*)(TArgs ...) noexcept> {
      using Type = TReturn (*)(TClass *, TArgs ...) noexcept;
      using Handle = TClass *;
    };

    template<class TClass, class TReturn, class ...TArgs>
    struct MethodPtrToMethodProxyPtrImpl<TReturn (TClass::*)(TArgs ...) const noexcept> {
      using Type = TReturn (*)(const WeakRef<const TClass> *, TArgs ...) noexcept;
      using Handle = const TClass *;
    };

    template<MethodPtrType T>
    using MethodPtrToMethodProxyPtr = typename MethodPtrToMethodProxyPtrImpl<T>::Type;
  }

  template<class T>
  class AsTypeBuilder;

  template<MethodPtrType auto method>
  auto makeProxy() -> priv::MethodPtrToMethodProxyPtr<decltype(method)> {
    using Method = decltype(method);
    using ProxyPtr = priv::MethodPtrToMethodProxyPtr<Method>;
    using Handle = typename priv::MethodPtrToMethodProxyPtrImpl<Method>::Handle;

    ProxyPtr r = []<class ...TArgs>(Handle handle, TArgs && ...args)
      -> typename function_traits<ProxyPtr>::return_type {
      return (handle->*method)(std::forward<TArgs>(args) ...);
    };

    return r;
  }
} // px::script

#endif //PX_SCRIPT_TYPE_BUILDER_COMMON_HPP
