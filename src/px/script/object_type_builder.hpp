// Copyright (c) 2023.

//
// Created by pyxiion on 14.12.23.
//

#ifndef PX_ENGINE_OBJECT_TYPE_BUILDER_HPP
#define PX_ENGINE_OBJECT_TYPE_BUILDER_HPP
#include <string>
#include <type_traits>
#include <memory>
#include <functional>
#include <new>
#include "common.hpp"
#include "template/signatures.hpp"

namespace px::script {
  namespace priv {
    // Object type flags
    namespace ObjTypeFlags {
      enum Enum {
        Reference                        = (1<<0),
        Value                            = (1<<1),
        GC                               = (1<<2),
        POD                              = (1<<3),
        NoHandle                         = (1<<4),
        Scoped                           = (1<<5),
        Template                         = (1<<6),
        AsHandle                         = (1<<7),
        Class                            = (1<<8),
        ClassConstructor                 = (1<<9),
        ClassDestructor                  = (1<<10),
        ClassAssigment                   = (1<<11),
        ClassCopyConstructor             = (1<<12),
        ClassMoreConstructors            = (1<<31),
        Primitive                        = (1<<13),
        Float                            = (1<<14),
        Array                            = (1<<15),
        AllInts                          = (1<<16),
        AllFloats                        = (1<<17),
        NoCount                          = (1<<18),
        Align8                           = (1<<19),
        ImplicitHandle                   = (1<<20)
      };
    } // ObjTypeFlags

    // Behaviours
    namespace ObjBehaviour
    {
      enum Enum {
        // Value object memory management
        Construct,
        ListConstruct,
        Destruct,

        // Reference object memory management
        Factory,
        ListFactory,
        AddRef,
        Release,
        GetWeakrefFlag,

        // Object operators
        TemplateCallback
      };
    }

    // Smart ptr concept
    template<class T>
    struct SmartPtrImpl : std::false_type {};

    template<class T>
    struct SmartPtrImpl<std::shared_ptr<T>> : std::true_type {};

    template<class T>
    struct SmartPtrImpl<std::unique_ptr<T>> : std::true_type {};

    template<class T>
    concept SmartPtr = SmartPtrImpl<T>::value;

    // AS binding
    void registerClassType(asIScriptEngine *engine, const std::string &name, int size, ObjTypeFlags::Enum flags);

    void registerClassBehaviour(asIScriptEngine *engine, const std::string &name, ObjBehaviour::Enum behaviour, const std::string &declaration, void *funcPtr);

    struct DummyClass {};
    typedef void (DummyClass::*DummyMethod)();

    template<class T>
    inline decltype(auto) makeDummyMethod(T &t) {
      return reinterpret_cast<DummyMethod>(t);
    };

    void registerMethod(asIScriptEngine *engine, const std::string &name, const std::string &declaration, DummyMethod funcPtr);

    void registerProxyMethod(asIScriptEngine *engine, const std::string &name, const std::string &declaration, void *funcPtr);

    void registerProperty(asIScriptEngine *engine, const std::string &name, const std::string &declaration, int offset);

    // Class helpers

    template<class T>
    struct PtrHolder {
      constexpr PtrHolder(T ptr) : ptr(ptr) {}

      T ptr;

      inline operator T() const { return ptr; }
    };

    template<class, class ...>
    struct MethodImpl;

    template<class T, class TReturn, class ...TArgs>
    struct MethodImpl<TReturn (T::*)(TArgs...)> : public PtrHolder<TReturn (T::*)(TArgs...)> {
    };

    template<class T, class TReturn, class ...TArgs>
    struct MethodImpl<TReturn (T::*)(TArgs...) const> : public PtrHolder<TReturn (T::*)(TArgs...) const> {
    };

    template<class T>
    struct Method : public MethodImpl<T> {
      constexpr Method(T ptr) : MethodImpl<T>(ptr) {}
    };

    template<class T, class U>
    int getPropertyOffset(U property) {
      return (int)(size_t)(&(reinterpret_cast<T*>(100000)->*property))-100000;
    }

    template<class>
    struct FieldImpl;

    template<class T, class U>
    struct FieldImpl<U (T::*)> : public PtrHolder<U(T::*)> {
      using PtrType = U (T::*);
      using PtrHolder<PtrType>::ptr;

      int getOffset() const {
        return getPropertyOffset<T, PtrType>(ptr);
      };
    };

    template<class T, class U>
    struct FieldImpl<const U (T::*)> : public PtrHolder<const U (T::*)> {
      using PtrType = const U (T::*);
      using PtrHolder<PtrType>::ptr;

      int getOffset() const {
        return getPropertyOffset<T, PtrType>(ptr);
      };
    };

    template<class T>
    struct Field : public FieldImpl<T> {
      constexpr Field(T ptr) : FieldImpl<T>(ptr) {}
    };

    template<class T>
    struct TypeTraits {
      constexpr static inline bool hasConstructor        = std::is_default_constructible_v<T> && not std::is_trivially_constructible_v<T>;
      constexpr static inline bool hasDestructor         = std::is_destructible_v<T>          && not std::is_trivially_destructible_v<T>;
      constexpr static inline bool hasAssignmentOperator = std::is_copy_assignable_v<T>       && not std::is_trivially_copy_assignable_v<T>;
      constexpr static inline bool hasCopyConstructor    = std::is_copy_constructible_v<T>    && not std::is_trivially_copy_constructible_v<T>;

      constexpr static inline bool isFloat     = std::is_floating_point_v<T>;
      constexpr static inline bool isPrimitive = std::is_integral_v<T> or std::is_pointer_v<T> or std::is_enum_v<T>;
      constexpr static inline bool isClass     = std::is_class_v<T>;
      constexpr static inline bool isArray     = std::is_array_v<T>;

      constexpr static inline bool isAlign8 = alignof(T) == 8;

      template<class TReturn, class ...TArgs>
      using MethodType = TReturn (T::*)(TArgs...);

      template<class U>
      using FieldType = U (T::*);
    };

    template<class T>
    inline constexpr int getClassFlags() {
      typedef TypeTraits<T> Traits;

      if constexpr (Traits::isFloat)
        return ObjTypeFlags::Float;
      else if constexpr (Traits::isPrimitive)
        return ObjTypeFlags::Primitive;

      else if constexpr (Traits::isClass) {
        int flags = ObjTypeFlags::Class;
        if constexpr (Traits::hasConstructor)
          flags |= ObjTypeFlags::ClassConstructor;
        if constexpr (Traits::hasDestructor)
          flags |= ObjTypeFlags::ClassDestructor;
        if constexpr (Traits::hasAssignmentOperator)
          flags |= ObjTypeFlags::ClassAssigment;
        if constexpr (Traits::hasCopyConstructor)
          flags |= ObjTypeFlags::ClassCopyConstructor;
        if constexpr (Traits::isAlign8)
          flags |= ObjTypeFlags::Align8;
        return flags;
      }
      else if constexpr (Traits::isArray) {
        return ObjTypeFlags::Array;
      }

      // unknown
      return 0;
    }

    template<class T>
    struct FunctionSignatureImpl;

    template<class TReturn, class ...TArgs>
    struct FunctionSignatureImpl<std::function<TReturn (TArgs...)>> {
      typedef TReturn (*Signature)(TArgs...);
    };

    template<class T>
    using FunctionSignature = typename FunctionSignatureImpl<T>::Signature;

    // Helpers
    template<class T, class ...TArgs>
    void Constructor(void *memory, TArgs ...args) {
      new (memory) T(std::forward<TArgs>(args)...);
    }

//    template<class T>
//    void CopyConstructor(void *memory, TArgs ...args) {
//      new (memory) T(std::forward<TArgs>(args)...);
//    }

    template<class T>
    void Destructor(void *memory) {
      reinterpret_cast<T *>(memory)->~T();
    }
  }

  template<class T>
  concept AsTypeValue = not std::is_pointer_v<T>;

  template<class T>
  class AsTypeBuilder;

  template<class T>
  class AsTypeBuilder {
  private:
    using ThisType = AsTypeBuilder<T>;
    using Traits = priv::TypeTraits<T>;

    template<class Return, class ...Args>
    using ProxyFunction = Return (*)(Args...);

  public:
    explicit AsTypeBuilder(asIScriptEngine *engine, const std::string &name)
      : m_engine(std::forward<asIScriptEngine *>(engine))
      , m_name(std::forward<decltype(name)>(name)) {
      priv::registerClassType(m_engine, std::string(m_name), sizeof(T), static_cast<priv::ObjTypeFlags::Enum>(priv::ObjTypeFlags::Value | priv::getClassFlags<T>()));
      registerBehaviour();
    }

    // Method
    template<priv::Method Method>
    ThisType &registerMethod(std::string &&name) {
      const std::string declaration = getSignature<decltype(Method.ptr)>(std::forward<decltype(name)>(name));
      priv::registerMethod(m_engine, m_name, declaration, priv::makeDummyMethod(Method.ptr));

      return *this;
    };

    template<class TReturn, class ...TArgs>
    ThisType &registerProxyMethod(std::string_view &&name, ProxyFunction<TReturn, T*, TArgs...> proxy) {
      const std::string declaration = getSignature<TReturn (*)(TArgs...)>(std::forward<decltype(name)>(name));
      priv::registerProxyMethod(m_engine, m_name, declaration, reinterpret_cast<void*>(proxy));

      return *this;
    }

    // Property

    template<priv::Field Field>
    ThisType &registerProperty(std::string_view &&name) {
      const std::string declaration = getPropertySignature<decltype(Field.ptr)>(std::forward<decltype(name)>(name));
      priv::registerProperty(m_engine, m_name, declaration, Field.getOffset());
      return *this;
    };

  private:
    asIScriptEngine *m_engine;
    const std::string m_name;

    inline void registerBehaviour() {
      // default empty constructor
      if constexpr (Traits::hasConstructor) {
        priv::registerClassBehaviour(m_engine, m_name, priv::ObjBehaviour::Construct, "void f()", reinterpret_cast<void *>(&priv::Constructor<T>));
      }
//      if constexpr (Traits::hasCopyConstructor) {
//        priv::registerClassBehaviour(m_engine, m_name, priv::ObjBehaviour::Construct, "void f()", reinterpret_cast<void *>(&priv::Constructor<T>));
//      }

      // destructor
      priv::registerClassBehaviour(m_engine, m_name, priv::ObjBehaviour::Destruct, "void f()", reinterpret_cast<void *>(&priv::Destructor<T>));
    }
  };

  // TODO for smart pointer
  template<class T>
  struct AsTypeBuilder<std::shared_ptr<T>> {

  };
} // px::script

#endif //PX_ENGINE_OBJECT_TYPE_BUILDER_HPP
