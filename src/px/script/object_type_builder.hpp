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
#include "easylogging++.h"
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
      using PtrHolder<TReturn (T::*)(TArgs...)>::ptr;

      template<class U>
      using ProxyPointerType = TReturn (*)(U, TArgs...);
    };

    template<class T, class TReturn, class ...TArgs>
    struct MethodImpl<TReturn (T::*)(TArgs...) const> : public PtrHolder<TReturn (T::*)(TArgs...) const> {
      using PtrHolder<TReturn (T::*)(TArgs...) const>::ptr;

      template<class U>
      using ProxyPointerType = TReturn (*)(U, TArgs...);
    };

    template<class T>
    struct Method : public MethodImpl<T> {
      constexpr Method(T ptr) : MethodImpl<T>(ptr) {}

      using MethodImpl<T>::ptr;
      using typename MethodImpl<T>::ProxyPointerType;
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

      [[nodiscard]] int getOffset() const {
        return getPropertyOffset<T, PtrType>(ptr);
      };
    };

    template<class T, class U>
    struct FieldImpl<const U (T::*)> : public PtrHolder<const U (T::*)> {
      using PtrType = const U (T::*);
      using PtrHolder<PtrType>::ptr;

      [[nodiscard]] int getOffset() const {
        return getPropertyOffset<T, PtrType>(ptr);
      };
    };

    template<class T>
    struct Field : public FieldImpl<T> {
      constexpr Field(T ptr) : FieldImpl<T>(ptr) {}
    };

    template<class T>
    struct Field<FieldImpl<T>> : public FieldImpl<T> {
      constexpr Field(T ptr) : FieldImpl<T>(ptr) {}
    };

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
        if constexpr (Traits::hasDefaultConstructor)
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

    template<class T>
    void CopyConstructor(void *memory, const T &other) {
      new (memory) T(other);
    }

    template<class T>
    void Destructor(void *memory) {
      reinterpret_cast<T *>(memory)->~T();
    }

    template<class T, class U = T>
    T &assigment(T& self, const U &other) {
      return self = other;
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
    explicit AsTypeBuilder(asIScriptEngine *engine, std::string &&name)
        : m_engine(std::forward<asIScriptEngine *>(engine))
        , m_name(std::forward<decltype(name)>(name)) {
      CLOG(INFO, "AngelScript") << "New AngelScript type \"" << m_name << "\"" << " with size " << sizeof(T);
      priv::registerClassType(m_engine, std::string(m_name), sizeof(T), static_cast<priv::ObjTypeFlags::Enum>(priv::ObjTypeFlags::Value | priv::getClassFlags<T>()));
      registerBehaviour();
    }
    explicit AsTypeBuilder(asIScriptEngine *&&engine)
        : AsTypeBuilder(std::forward<decltype(engine)>(engine), std::string(getTypeAsName<T>())) {}

    // Method
    template<priv::Method Method>
    ThisType &method(std::string &&name) {
      const std::string declaration = getSignature<decltype(Method.ptr)>(std::forward<decltype(name)>(name));
      priv::registerMethod(m_engine, m_name, declaration, priv::makeDummyMethod(Method.ptr));

      CLOG(INFO, "AngelScript") << "\t" << declaration;
      if constexpr (sizeof(Method.ptr) == 8) {
        CLOG(INFO, "AngelScript") << "\t\t at " << reinterpret_cast<void*>(Method.ptr);
      } else {
        union { decltype(&Method.ptr) mptr; void *raw[2]; };
        CLOG(INFO, "AngelScript") << "\t\t at " << raw[0] << " " << raw[1];
      }

      return *this;
    };

    template<class TReturn, class ...TArgs>
    ThisType &proxyMethod(std::string_view &&name, ProxyFunction<TReturn, T*, TArgs...> proxy) {
      const std::string declaration = getSignature<TReturn (*)(TArgs...)>(std::forward<decltype(name)>(name));
      priv::registerProxyMethod(m_engine, m_name, declaration, reinterpret_cast<void*>(proxy));

      CLOG(INFO, "AngelScript") << "\t" << declaration;
      CLOG(INFO, "AngelScript") << "\t\t at " << reinterpret_cast<void*>(proxy) << " (proxy)";

      return *this;
    }

    // Property

    template<priv::Field Field>
    ThisType &property(std::string_view &&name) {
      const std::string declaration = getPropertySignature<decltype(Field.ptr)>(std::forward<decltype(name)>(name));
      priv::registerProperty(m_engine, m_name, declaration, Field.getOffset());

      CLOG(INFO, "AngelScript") << "\t" << declaration;
      CLOG(INFO, "AngelScript") << "\t\t with offset " << Field.getOffset();
      return *this;
    };

  private:
    asIScriptEngine *m_engine;
    std::string m_name;

    inline void registerBehaviour() {
      CLOG(INFO, "AngelScript") << "\tBehaviour:";
      // default empty constructor
      if constexpr (Traits::hasDefaultConstructor) {
        priv::registerClassBehaviour(m_engine, m_name, priv::ObjBehaviour::Construct, "void f()", reinterpret_cast<void *>(&priv::Constructor<T>));
        CLOG(INFO, "AngelScript") << "\t\t+ Empty constructor";
      }
      // copy constructor
      if constexpr (Traits::hasCopyConstructor) {
        priv::registerClassBehaviour(m_engine, m_name, priv::ObjBehaviour::Construct, fmt::format("void f(const {} &in)", m_name), reinterpret_cast<void *>(&priv::CopyConstructor<T>));
        CLOG(INFO, "AngelScript") << "\t\t+ Copy constructor";
      }

      // assigment operator
      if constexpr (Traits::hasAssignmentOperator) {
        priv::registerProxyMethod(m_engine, m_name, fmt::format("{0} &opAssign(const {0} &in)", m_name), reinterpret_cast<void *>(&priv::assigment<T>));
        CLOG(INFO, "AngelScript") << "\t\t+ Assigment operator";
      }

      // destructor
      priv::registerClassBehaviour(m_engine, m_name, priv::ObjBehaviour::Destruct, "void f()", reinterpret_cast<void *>(&priv::Destructor<T>));
      CLOG(INFO, "AngelScript") << "\t\t+ Destructor";
    }
  };

  // SHARED PTR

  namespace priv {
    extern void setCtxException(const char *msg);

    template<class T>
    struct DefaultValue {
      inline static T f() { return T(); }
    };
    template<class T>
    struct DefaultValue<T&> {
      inline static T &f() { return *static_cast<T *>(0); }
    };
  }

  // Smart pointer
  template<class T>
  struct AsTypeBuilder<std::shared_ptr<T>> {
  private:
    using ThisType = AsTypeBuilder<std::shared_ptr<T>>;
    using Self = std::shared_ptr<T>;
    using Traits = priv::TypeTraits<Self>;

    template<class Return, class ...Args>
    using ProxyFunction = Return (*)(Args...);

  public:
    explicit AsTypeBuilder(asIScriptEngine *engine)
        : AsTypeBuilder(std::forward<decltype(engine)>(engine), std::string(getTypeAsName<Self>())) {}

    explicit AsTypeBuilder(asIScriptEngine *engine, std::string &&name)
        : m_engine(std::forward<asIScriptEngine *>(engine))
        , m_name(std::forward<decltype(name)>(name)) {
      CLOG(INFO, "AngelScript") << "New AngelScript type \"" << m_name << "\" (smart pointer)" << " with size " << sizeof(Self) << " (" << sizeof(T) << ")";
      priv::registerClassType(m_engine, m_name, sizeof(Self), static_cast<priv::ObjTypeFlags::Enum>(priv::ObjTypeFlags::Value | priv::getClassFlags<T>()));
      registerBehaviour();
    }

    // Deriving
    template<class Base>
    ThisType &derived() {
      static_assert(std::is_base_of_v<Base, T>, "Base should be the parent of the type.");

      using ThisPtr = std::shared_ptr<T>;
      using BasePtr = std::shared_ptr<Base>;

      auto basePtrName = getTypeAsName<BasePtr>();

      using ToBasePtr = BasePtr (*)(ThisPtr &&) noexcept;
      using ToThisPtr = ThisPtr (*)(BasePtr &&) noexcept;

      CLOG(INFO, "AngelScript") << "\tderived from " << basePtrName;

      // to this conversion
      std::string decl = fmt::format("{} opCast() const", m_name);
      void *funcPtr = reinterpret_cast<void*>(static_cast<ToThisPtr>(&std::dynamic_pointer_cast<T, Base>));

      priv::registerProxyMethod(m_engine, std::string(basePtrName), decl, funcPtr);
      CLOG(INFO, "AngelScript") << "\t\t " << decl;
      CLOG(INFO, "AngelScript") << "\t\t\tat " << funcPtr;

      // to base conversion
      decl = fmt::format("{} opImplCast() const", basePtrName);
      funcPtr = reinterpret_cast<void*>(static_cast<ToBasePtr>(&std::dynamic_pointer_cast<Base, T>));

      priv::registerProxyMethod(m_engine, m_name, decl, funcPtr);
      CLOG(INFO, "AngelScript") << "\t\t " << decl;
      CLOG(INFO, "AngelScript") << "\t\t\tat " << funcPtr;

      return *this;
    };


    // Method
    template<priv::Method Method>
    ThisType &method(std::string &&name) {
      typedef decltype(Method) TMethod;
      typedef decltype(Method.ptr) TMethodPtr;
      const std::string declaration = getSignature<TMethodPtr>(std::forward<decltype(name)>(name));

      typedef typename TMethod::template ProxyPointerType<Self> Ptr;

      Ptr proxy = [](Self self, auto ...args) {
        if (not self) {
          priv::setCtxException("Attempting to call method of a null pointer");
          fmt::print("Attempting to call method of a null pointer\n");
          return;
        }
        ((*self).*(Method.ptr))(args...);
      };

      priv::registerProxyMethod(m_engine, m_name, declaration, reinterpret_cast<void *>(proxy));

      CLOG(INFO, "AngelScript") << "\t" << declaration;
      CLOG(INFO, "AngelScript") << "\t\tat " << reinterpret_cast<void*>(proxy);

      return *this;
    };

    // Property

    template<priv::Field Field>
    ThisType &property(std::string_view name) {
      typedef decltype(Field.ptr) TField;

      // getter
      typedef px::field_traits<TField> FieldTraits;
      typedef typename FieldTraits::type Type;

      using GetterPtr = Type (*)(const Self &);
      using RefGetterPtr = Type& (*)(Self &);

      using SetterPtr = void (*)(Self &, const Type &);


      const std::string declaration = getPropertySignature<decltype(Field.ptr)>(std::forward<decltype(name)>(name));
      CLOG(INFO, "AngelScript") << "\t" << declaration;

      auto getDecl = fmt::format("{} get_{}() const property", getTypeAsName<Type>(), name);
      GetterPtr getter = [](const Self &self) -> Type {
        if (not self) {
          priv::setCtxException("Attempting to access member of a null pointer");
          fmt::print("Attempting to access member of a null pointer (getter, ptr: {})\n", fmt::ptr(self.get()));
          return priv::DefaultValue<Type>::f();
        }
        return (self.get())->*(Field.ptr);
      };

      priv::registerProxyMethod(m_engine, m_name, getDecl, reinterpret_cast<void *>(getter));
      CLOG(INFO, "AngelScript") << "\t\t proxied with:";
      CLOG(INFO, "AngelScript") << "\t\t\t " << getDecl << " at " << reinterpret_cast<void*>(getter);

      if constexpr (not FieldTraits::is_const) {
        auto getRefDecl = fmt::format("{}& get_{}() property", getTypeAsName<Type>(), name);
        RefGetterPtr refGetter = [](Self &self) -> Type& {
          if (not self) {
            priv::setCtxException("Attempting to access member of a null pointer");
            fmt::print("Attempting to access member of a null pointer (refGetter, ptr: {})\n", fmt::ptr(self.get()));
            return priv::DefaultValue<Type&>::f();
          }
          return (self.get())->*(Field.ptr);
        };

        auto setDecl = fmt::format("void set_{1}(const {0} &in) property", getTypeAsName<Type>(), name);
        SetterPtr refSetter = [](Self &self, const Type &other) -> void {
          if (not self) {
            priv::setCtxException("Attempting to access member of a null pointer");
            fmt::print("Attempting to access member of a null pointer (refSetter, ptr: {})\n", fmt::ptr(self.get()));
            return;
          }
          (self.get())->*(Field.ptr) = other;
        };

        priv::registerProxyMethod(m_engine, m_name, getRefDecl, reinterpret_cast<void *>(refGetter));
        priv::registerProxyMethod(m_engine, m_name, setDecl, reinterpret_cast<void *>(refSetter));

        CLOG(INFO, "AngelScript") << "\t\t\t " << getRefDecl << " at " << reinterpret_cast<void*>(refGetter);
        CLOG(INFO, "AngelScript") << "\t\t\t " << setDecl << " at " << reinterpret_cast<void*>(refSetter);
      }

      return *this;
    };

  private:
    asIScriptEngine *m_engine;
    std::string m_name;

    inline void registerBehaviour() {
      CLOG(INFO, "AngelScript") << "\tBehaviour:";
      // default empty constructor
      if constexpr (Traits::hasDefaultConstructor) {
        priv::registerClassBehaviour(m_engine, m_name, priv::ObjBehaviour::Construct, "void f()", reinterpret_cast<void *>(&priv::Constructor<Self>));
        CLOG(INFO, "AngelScript") << "\t\t+ Empty constructor";
      }
      // copy constructor
      if constexpr (Traits::hasCopyConstructor) {
        priv::registerClassBehaviour(m_engine, m_name, priv::ObjBehaviour::Construct, fmt::format("void f(const {} &in)", m_name), reinterpret_cast<void *>(&priv::CopyConstructor<Self>));
        CLOG(INFO, "AngelScript") << "\t\t+ Copy constructor";
      }

      // assigment operator
      if constexpr (Traits::hasAssignmentOperator) {
        priv::registerProxyMethod(m_engine, m_name, fmt::format("{0} &opAssign(const {0} &in)", m_name), reinterpret_cast<void *>(&priv::assigment<Self>));
        CLOG(INFO, "AngelScript") << "\t\t+ Assigment operator";
      }

      // destructor
      priv::registerClassBehaviour(m_engine, m_name, priv::ObjBehaviour::Destruct, "void f()", reinterpret_cast<void *>(&priv::Destructor<Self>));
      CLOG(INFO, "AngelScript") << "\t\t+ Destructor";
    }
  };

  // Reference (AngelScript won't count or instanciate it on its own)
  template<class T>
  struct AsTypeBuilder<T*> {
  private:
    typedef AsTypeBuilder<T*> ThisType;
    typedef T* Self;

    template<class Return, class ...Args>
    using ProxyFunction = Return (*)(Args...);

  public:

    explicit AsTypeBuilder(asIScriptEngine *engine)
        : AsTypeBuilder(std::forward<decltype(engine)>(engine), std::string(getTypeAsName<Self>())) {}

    explicit AsTypeBuilder(asIScriptEngine *engine, std::string &&name)
        : m_engine(std::forward<asIScriptEngine *>(engine))
        , m_name(std::forward<decltype(name)>(name)) {
      CLOG(INFO, "AngelScript") << "New AngelScript type \"" << m_name << "\" (reference, size " << sizeof(T) << "B)";
      priv::registerClassType(m_engine, m_name, 0, static_cast<priv::ObjTypeFlags::Enum>(priv::ObjTypeFlags::Reference | priv::ObjTypeFlags::NoCount));
    }

    // Same code (AsTypeBuilder<T>)
    // TODO refactor later

    // Method
    template<priv::Method Method>
    ThisType &method(std::string &&name) {
      const std::string declaration = getSignature<decltype(Method.ptr)>(std::forward<decltype(name)>(name));
      priv::registerMethod(m_engine, m_name, declaration, priv::makeDummyMethod(Method.ptr));

      CLOG(INFO, "AngelScript") << "\t" << declaration;
      if constexpr (sizeof(Method.ptr) == 8) {
        CLOG(INFO, "AngelScript") << "\t\t at " << reinterpret_cast<void*>(Method.ptr);
      } else {
        union { decltype(&Method.ptr) mptr; void *raw[2]; };
        CLOG(INFO, "AngelScript") << "\t\t at " << raw[0] << " " << raw[1];
      }

      return *this;
    };

    template<class TReturn, class ...TArgs>
    ThisType &proxyMethod(std::string_view &&name, ProxyFunction<TReturn, T*, TArgs...> proxy) {
      const std::string declaration = getSignature<TReturn (*)(TArgs...)>(std::forward<decltype(name)>(name));
      priv::registerProxyMethod(m_engine, m_name, declaration, reinterpret_cast<void*>(proxy));

      CLOG(INFO, "AngelScript") << "\t" << declaration;
      CLOG(INFO, "AngelScript") << "\t\t at " << reinterpret_cast<void*>(proxy) << " (proxy)";

      return *this;
    }

    // Property

    template<priv::Field Field>
    ThisType &property(std::string_view &&name) {
      const std::string declaration = getPropertySignature<decltype(Field.ptr)>(std::forward<decltype(name)>(name));
      priv::registerProperty(m_engine, m_name, declaration, Field.getOffset());

      CLOG(INFO, "AngelScript") << "\t" << declaration;
      CLOG(INFO, "AngelScript") << "\t\t with offset " << Field.getOffset();
      return *this;
    };

  private:
    asIScriptEngine *m_engine;
    std::string m_name;


  };
} // px::script

#endif //PX_ENGINE_OBJECT_TYPE_BUILDER_HPP
