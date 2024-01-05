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
        Reference             = (1 << 0),
        Value                 = (1 << 1),
        GC                    = (1 << 2),
        POD                   = (1 << 3),
        NoHandle              = (1 << 4),
        Scoped                = (1 << 5),
        Template              = (1 << 6),
        AsHandle              = (1 << 7),
        Class                 = (1 << 8),
        ClassConstructor      = (1 << 9),
        ClassDestructor       = (1 << 10),
        ClassAssigment        = (1 << 11),
        ClassCopyConstructor  = (1 << 12),
        ClassMoreConstructors = (1 << 31),
        Primitive             = (1 << 13),
        Float                 = (1 << 14),
        Array                 = (1 << 15),
        AllInts               = (1 << 16),
        AllFloats             = (1 << 17),
        NoCount               = (1 << 18),
        Align8                = (1 << 19),
        ImplicitHandle        = (1 << 20)
      };
    } // ObjTypeFlags

    // Behaviours
    namespace ObjBehaviour {
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

    void registerClassBehaviour(asIScriptEngine *engine, const std::string &name, ObjBehaviour::Enum behaviour,
                                const std::string &declaration, void *funcPtr);

    struct DummyClass {};

    typedef void (DummyClass::*DummyMethod)();

    template<class T>
    decltype(auto) makeDummyMethod(T &t) {
      return reinterpret_cast<DummyMethod>(t);
    }

    void registerMethod(asIScriptEngine *engine, const std::string &name, const std::string &declaration,
                        DummyMethod funcPtr);

    void registerProxyMethod(asIScriptEngine *engine, const std::string &name, const std::string &declaration,
                             void *funcPtr);

    void registerCdeclMethod(asIScriptEngine *engine, const std::string &name, const std::string &declaration,
    void *funcPtr);

    void registerGenericMethod(asIScriptEngine *engine, const std::string &name, const std::string &declaration,
                             void *funcPtr, void *auxiliary);

    void registerProperty(asIScriptEngine *engine, const std::string &name, const std::string &declaration, int offset);

    void setCtxException(const char *msg);

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

    template<class T>
    constexpr int getClassFlags() {
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
      } else if constexpr (Traits::isArray) {
        return ObjTypeFlags::Array;
      }

      // unknown
      return 0;
    }

    // Helpers
    template<class T, class ...TArgs>
    void Constructor(void *memory, TArgs ...args) {
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

    template<class T, class U = T>
    T &assigment(T &self, const U &other) {
      return self = other;
    }
  }

  template<class T>
  concept AsTypeValue = not std::is_pointer_v<T>;

  namespace priv {
    template<class Handle>
    class AsTypeBuilderBase {
    public:
      explicit AsTypeBuilderBase(asIScriptEngine *engine)
        : AsTypeBuilderBase(engine, std::string(getTypeAsName<Handle>())) {}

      AsTypeBuilderBase(asIScriptEngine *engine, std::string &&name)
        : m_engine(engine)
      , m_name(std::move(name)) {
        CLOG(INFO, "AngelScript") << "New AngelScript type \"" << m_name << "\"" << " with size " << sizeof(Handle);
        priv::registerClassType(m_engine, std::string(m_name), sizeof(Handle),
                                static_cast<priv::ObjTypeFlags::Enum>(
                                  priv::ObjTypeFlags::Value | priv::getClassFlags<Handle>()));
        registerBehaviour();
      }

    protected:
      asIScriptEngine *m_engine;
      std::string m_name;

      void registerBehaviour() {
        using Traits = priv::TypeTraits<Handle>;
        CLOG(INFO, "AngelScript") << "\tBehaviour:";
        // default empty constructor
        if constexpr (Traits::hasDefaultConstructor) {
          priv::registerClassBehaviour(m_engine, m_name, priv::ObjBehaviour::Construct, "void f()",
                                       reinterpret_cast<void *>(&priv::Constructor<Handle>));
          CLOG(INFO, "AngelScript") << "\t\t+ Empty constructor";
        }
        // copy constructor
        if constexpr (Traits::hasCopyConstructor) {
          priv::registerClassBehaviour(m_engine, m_name, priv::ObjBehaviour::Construct,
                                       fmt::format("void f(const {} &in)", m_name),
                                       reinterpret_cast<void *>(&priv::CopyConstructor<Handle>));
          CLOG(INFO, "AngelScript") << "\t\t+ Copy constructor";
        }

        // assigment operator
        if constexpr (Traits::hasAssignmentOperator) {
          priv::registerProxyMethod(m_engine, m_name, fmt::format("{0} &opAssign(const {0} &in)", m_name),
                                    reinterpret_cast<void *>(&priv::assigment<Handle>));
          CLOG(INFO, "AngelScript") << "\t\t+ Assigment operator";
        }

        // destructor
        priv::registerClassBehaviour(m_engine, m_name, priv::ObjBehaviour::Destruct, "void f()",
                                     reinterpret_cast<void *>(&priv::Destructor<Handle>));
        CLOG(INFO, "AngelScript") << "\t\t+ Destructor";
      }
    };

    // Base is the type from which we register methods and fields.
    // Handle is a type where we register constructors, destructors and operators.
    template<class Base, class Handle = Base>
    class AsTypeBuilderCommon : public AsTypeBuilderBase<Handle> {
    public:
      // constructor
      using AsTypeBuilderBase<Handle>::AsTypeBuilderBase;

      template<MethodPtrTypeOf<Base> PtrType>
      void method(const std::string &&name, PtrType method) {
        const std::string declaration = getSignature<PtrType>(std::forward<decltype(name)>(name));
        priv::registerMethod(m_engine, m_name, declaration, priv::makeDummyMethod(method));

        CLOG(INFO, "AngelScript") << "\t" << declaration;
      }

      template<PropertyPtrTypeOf<Base> PtrType>
      void property(const std::string &&name, PtrType property) {
        const std::string declaration = getPropertySignature<PtrType>(std::forward<decltype(name)>(name));
        const std::size_t offset      = getPropertyOffset(property);
        priv::registerProperty(m_engine, m_name, declaration, static_cast<int>(offset));

        CLOG(INFO, "AngelScript") << "\t" << declaration;
        CLOG(INFO, "AngelScript") << "\t\twith offset " << offset;
      }

      template<FunctionSignature T>
      void proxy(const std::string &&name, T *proxy) {
        auto *ptr = reinterpret_cast<void *>(proxy);

        static_assert(FunctionSignatureWith<void (int *), int *>);

        constexpr bool hasPtrOrRef   = FunctionSignatureWith<T, Base *> or FunctionSignatureWith<T, Base &>;
        constexpr bool hasCPtrOrCRef = FunctionSignatureWith<T, const Base *> or FunctionSignatureWith<T, const Base &>;
        constexpr bool hasThis       = hasPtrOrRef or hasCPtrOrCRef;

        if constexpr (hasThis) {
          const std::string declaration = getProxySignature<T>(std::forward<decltype(name)>(name), hasCPtrOrCRef);
          priv::registerProxyMethod(m_engine, m_name, declaration, ptr);
          CLOG(INFO, "AngelScript") << "\t" << declaration << " (proxy with this)";
        } else {
          const std::string declaration = getSignature<T>(std::forward<decltype(name)>(name));
          priv::registerCdeclMethod(m_engine, m_name, declaration, ptr);
          CLOG(INFO, "AngelScript") << "\t" << declaration << " (cdecl proxy)";
        }
      }

    protected:
      using AsTypeBuilderBase<Handle>::m_engine;
      using AsTypeBuilderBase<Handle>::m_name;
    };

    template<MethodPtrType PtrType, class ...PrefixArgs>
    struct GetProxyTypeImpl;

    template<class TClass, class TReturn, class ...TArgs, class ...PrefixArgs>
    struct GetProxyTypeImpl<TReturn (TClass::*)(TArgs...), PrefixArgs...> {
      using Type = TReturn (*)(PrefixArgs..., TArgs...);
    };

    template<class TClass, class TReturn, class ...TArgs, class ...PrefixArgs>
    struct GetProxyTypeImpl<TReturn (TClass::*)(TArgs...) const, PrefixArgs...> {
      using Type = TReturn (*)(PrefixArgs..., TArgs...);
    };

    template<class T, class ...TArgs>
    using GetProxyType = typename GetProxyTypeImpl<T, TArgs...>::Type;

    // Base is the type from which we register methods and fields.
    // Handle is a smart ptr type
    template<class Base, SmartPointerTypeFor<Base> Handle>
    class AsTypeBuilderSmartPtr : public AsTypeBuilderBase<Handle> {
    public:
      using AsTypeBuilderBase<Handle>::AsTypeBuilderBase;

      template<MethodPtrTypeOf<Base> auto method>
      void method(const std::string &&name) {
        using PtrType = decltype(method);
        using Traits = function_traits<PtrType>;
        using ReturnType = typename Traits::return_type;

        using ProxyPtrType = GetProxyType<PtrType, const Handle &>;

        #pragma GCC diagnostic push
        #pragma GCC diagnostic ignored "-Wreturn-type"
        ProxyPtrType proxyPtr = []<class ...TArgs>(const Handle &handle, TArgs &&...args) -> ReturnType {
          if (not handle) {
            setCtxException("Calling nullptr object");
          } else {
            return (*handle.*method)(std::forward<TArgs>(args)...);
          }
        };
        #pragma GCC diagnostic pop

        std::string declaration = getProxySignature<ProxyPtrType>(name, Traits::is_const);
        registerProxyMethod(m_engine, m_name, declaration, reinterpret_cast<void*>(proxyPtr));

        CLOG(INFO, "AngelScript") << "\t" << declaration << " (proxied)";
      }

      template<PropertyPtrTypeOf<Base> PtrType>
      void property(const std::string &&name, PtrType propertyPtr) {
        using Traits = field_traits<PtrType>;

        getter<true>(name, propertyPtr);

        if constexpr (not Traits::is_const) {
          getter<false>(name, propertyPtr);
          setter(name, propertyPtr);
        }
      }

    protected:
      template<bool isConst, PropertyPtrTypeOf<Base> PtrType>
      void getter(const std::string &name, PtrType property) {
        using Traits = field_traits<PtrType>;
        using Type = std::remove_const_t<typename Traits::type>;
        auto offset = reinterpret_cast<void*>(getPropertyOffset(property));

        static_assert(Traits::is_const <= isConst);
        constexpr std::string_view pattern = [] {
          if constexpr (isConst) {
            return "const {}& get_{}() const property";
          } else {
            return "{}& get_{}() property";
          }
        }();


        auto declaration = fmt::format(pattern, getTypeAsName<Type>(), name);
        registerGenericMethod(m_engine, m_name, declaration, reinterpret_cast<void*>(&getterGenericProxy), offset);

        CLOG(INFO, "AngelScript") << "\t" << declaration;
      }

      template<PropertyPtrTypeOf<Base> PtrType>
      void setter(const std::string &name, PtrType property) {
        using Type = typename field_traits<PtrType>::type;
        auto offset = reinterpret_cast<void*>(getPropertyOffset(property));

        auto declaration = fmt::format("void set_{1}({0}) property", getTypeAsName<Type>(), name);
        registerGenericMethod(m_engine, m_name, declaration, reinterpret_cast<void*>(&setterGenericProxy<Type>), offset);

        CLOG(INFO, "AngelScript") << "\t" << declaration;
      }

      static void getterGenericProxy(GenericManipulator generic) {
        const auto offset = reinterpret_cast<std::ptrdiff_t>(generic.getAuxiliary());
        auto *handle = static_cast<Handle *>(generic.getObject());

        generic.setReturnAddress(reinterpret_cast<std::byte *>(handle->get()) + offset);
      }

      template<class Type>
      static void setterGenericProxy(GenericManipulator generic) {
        const auto offset = reinterpret_cast<std::ptrdiff_t>(generic.getAuxiliary());
        auto *handle = static_cast<Handle *>(generic.getObject());

        auto *value = reinterpret_cast<Type *>(reinterpret_cast<std::byte *>(handle->get()) + offset);
        auto newValue = generic.getArg<Type>(0);
        *value = newValue;
      }

      using AsTypeBuilderBase<Handle>::m_engine;
      using AsTypeBuilderBase<Handle>::m_name;
    };
  }

  template<class T>
  class AsTypeBuilder;

  template<class T>
  class AsTypeBuilder : public priv::AsTypeBuilderCommon<T> {
  public:
    using priv::AsTypeBuilderCommon<T>::AsTypeBuilderCommon;

    using priv::AsTypeBuilderCommon<T>::method;
    using priv::AsTypeBuilderCommon<T>::proxy;
    using priv::AsTypeBuilderCommon<T>::property;
  };

  template<class T>
  class AsTypeBuilder<std::shared_ptr<T>> : public priv::AsTypeBuilderSmartPtr<T, std::shared_ptr<T>> {
  public:
    using priv::AsTypeBuilderSmartPtr<T, std::shared_ptr<T>>::AsTypeBuilderSmartPtr;

    using priv::AsTypeBuilderSmartPtr<T, std::shared_ptr<T>>::method;
    using priv::AsTypeBuilderSmartPtr<T, std::shared_ptr<T>>::property;
  };

  template<class T>
  class AsTypeBuilder<T *>{
  public:
  };

  // // SHARED PTR
  //
  // namespace priv {
  //   extern void setCtxException(const char *msg);
  //
  //   template<class T>
  //   struct DefaultValue {
  //     inline static T f() { return T(); }
  //   };
  //
  //   template<class T>
  //   struct DefaultValue<T &> {
  //     inline static T &f() { return *static_cast<T *>(reinterpret_cast<void *>(1)); }
  //   };
  // }
  //
  // // Smart pointer
  // template<class T>
  // struct AsTypeBuilder<std::shared_ptr<T> > {
  // private:
  //   using ThisType = AsTypeBuilder<std::shared_ptr<T> >;
  //   using Self = std::shared_ptr<T>;
  //   using Traits = priv::TypeTraits<Self>;
  //
  //   template<class Return, class... Args>
  //   using ProxyFunction = Return (*)(Args...);
  //
  // public:
  //   explicit AsTypeBuilder(asIScriptEngine *engine)
  //     : AsTypeBuilder(std::forward<decltype(engine)>(engine), std::string(getTypeAsName<Self>())) {
  //   }
  //
  //   explicit AsTypeBuilder(asIScriptEngine *engine, std::string &&name)
  //     : m_engine(std::forward<asIScriptEngine *>(engine))
  //       , m_name(std::forward<decltype(name)>(name)) {
  //     CLOG(INFO, "AngelScript") << "New AngelScript type \"" << m_name << "\" (smart pointer)" << " with size " <<
  //         sizeof(Self) << " (" << sizeof(T) << ")";
  //     priv::registerClassType(m_engine, m_name, sizeof(Self),
  //                             static_cast<priv::ObjTypeFlags::Enum>(
  //                               priv::ObjTypeFlags::Value | priv::getClassFlags<T>()));
  //     registerBehaviour();
  //   }
  //
  //   // Deriving
  //   template<class Base>
  //   ThisType &derived() {
  //     static_assert(std::is_base_of_v<Base, T>, "Base should be the parent of the type.");
  //
  //     using ThisPtr = std::shared_ptr<T>;
  //     using BasePtr = std::shared_ptr<Base>;
  //
  //     auto basePtrName = getTypeAsName<BasePtr>();
  //
  //     using ToBasePtr = BasePtr (*)(ThisPtr &&) noexcept;
  //     using ToThisPtr = ThisPtr (*)(BasePtr &&) noexcept;
  //
  //     CLOG(INFO, "AngelScript") << "\tderived from " << basePtrName;
  //
  //     // to this conversion
  //     std::string decl = fmt::format("{} opCast() const", m_name);
  //     void *funcPtr = reinterpret_cast<void *>(static_cast<ToThisPtr>(&std::dynamic_pointer_cast<T, Base>));
  //
  //     priv::registerProxyMethod(m_engine, std::string(basePtrName), decl, funcPtr);
  //     CLOG(INFO, "AngelScript") << "\t\t " << decl;
  //     CLOG(INFO, "AngelScript") << "\t\t\tat " << funcPtr;
  //
  //     // to base conversion
  //     decl = fmt::format("{} opImplCast() const", basePtrName);
  //     funcPtr = reinterpret_cast<void *>(static_cast<ToBasePtr>(&std::dynamic_pointer_cast<Base, T>));
  //
  //     priv::registerProxyMethod(m_engine, m_name, decl, funcPtr);
  //     CLOG(INFO, "AngelScript") << "\t\t " << decl;
  //     CLOG(INFO, "AngelScript") << "\t\t\tat " << funcPtr;
  //
  //     return *this;
  //   };
  //
  //
  //   // Method
  //   template<priv::Method Method>
  //   ThisType &method(std::string &&name) {
  //     typedef decltype(Method) TMethod;
  //     typedef decltype(Method.ptr) TMethodPtr;
  //     const std::string declaration = getSignature<TMethodPtr>(std::forward<decltype(name)>(name));
  //
  //     typedef typename TMethod::template ProxyPointerType<Self> Ptr;
  //
  //     Ptr proxy = [](Self self, auto... args) {
  //       if (not self) {
  //         priv::setCtxException("Attempting to call method of a null pointer");
  //         fmt::print("Attempting to call method of a null pointer\n");
  //         return;
  //       }
  //       ((*self).*(Method.ptr))(args...);
  //     };
  //
  //     priv::registerProxyMethod(m_engine, m_name, declaration, reinterpret_cast<void *>(proxy));
  //
  //     CLOG(INFO, "AngelScript") << "\t" << declaration;
  //     CLOG(INFO, "AngelScript") << "\t\tat " << reinterpret_cast<void *>(proxy);
  //
  //     return *this;
  //   };
  //
  //   // Property
  //
  //   template<priv::Field Field>
  //   ThisType &property(std::string_view name) {
  //     typedef decltype(Field.ptr) TField;
  //
  //     // getter
  //     typedef px::field_traits<TField> FieldTraits;
  //     typedef typename FieldTraits::type Type;
  //
  //     using GetterPtr = Type (*)(const Self &);
  //     using RefGetterPtr = Type& (*)(Self &);
  //
  //     using SetterPtr = void (*)(Self &, const Type &);
  //
  //
  //     const std::string declaration = getPropertySignature<decltype(Field.ptr)>(std::forward<decltype(name)>(name));
  //     CLOG(INFO, "AngelScript") << "\t" << declaration;
  //
  //     auto getDecl = fmt::format("{} get_{}() const property", getTypeAsName<Type>(), name);
  //     GetterPtr getter = [](const Self &self) -> Type {
  //       if (not self) {
  //         priv::setCtxException("Attempting to access member of a null pointer");
  //         fmt::print("Attempting to access member of a null pointer (getter, ptr: {})\n", fmt::ptr(self.get()));
  //         return priv::DefaultValue<Type>::f();
  //       }
  //       return (self.get())->*(Field.ptr);
  //     };
  //
  //     priv::registerProxyMethod(m_engine, m_name, getDecl, reinterpret_cast<void *>(getter));
  //     CLOG(INFO, "AngelScript") << "\t\t proxied with:";
  //     CLOG(INFO, "AngelScript") << "\t\t\t " << getDecl << " at " << reinterpret_cast<void *>(getter);
  //
  //     if constexpr (not FieldTraits::is_const) {
  //       auto getRefDecl = fmt::format("{}& get_{}() property", getTypeAsName<Type>(), name);
  //       RefGetterPtr refGetter = [](Self &self) -> Type &{
  //         if (not self) {
  //           priv::setCtxException("Attempting to access member of a null pointer");
  //           fmt::print("Attempting to access member of a null pointer (refGetter, ptr: {})\n", fmt::ptr(self.get()));
  //           return priv::DefaultValue<Type &>::f();
  //         }
  //         return (self.get())->*(Field.ptr);
  //       };
  //
  //       auto setDecl = fmt::format("void set_{1}(const {0} &in) property", getTypeAsName<Type>(), name);
  //       SetterPtr refSetter = [](Self &self, const Type &other) -> void {
  //         if (not self) {
  //           priv::setCtxException("Attempting to access member of a null pointer");
  //           fmt::print("Attempting to access member of a null pointer (refSetter, ptr: {})\n", fmt::ptr(self.get()));
  //           return;
  //         }
  //         (self.get())->*(Field.ptr) = other;
  //       };
  //
  //       priv::registerProxyMethod(m_engine, m_name, getRefDecl, reinterpret_cast<void *>(refGetter));
  //       priv::registerProxyMethod(m_engine, m_name, setDecl, reinterpret_cast<void *>(refSetter));
  //
  //       CLOG(INFO, "AngelScript") << "\t\t\t " << getRefDecl << " at " << reinterpret_cast<void *>(refGetter);
  //       CLOG(INFO, "AngelScript") << "\t\t\t " << setDecl << " at " << reinterpret_cast<void *>(refSetter);
  //     }
  //
  //     return *this;
  //   };
  //
  // private:
  //   asIScriptEngine *m_engine;
  //   std::string m_name;
  //
  //   inline void registerBehaviour() {
  //     CLOG(INFO, "AngelScript") << "\tBehaviour:";
  //     // default empty constructor
  //     if constexpr (Traits::hasDefaultConstructor) {
  //       priv::registerClassBehaviour(m_engine, m_name, priv::ObjBehaviour::Construct, "void f()",
  //                                    reinterpret_cast<void *>(&priv::Constructor<Self>));
  //       CLOG(INFO, "AngelScript") << "\t\t+ Empty constructor";
  //     }
  //     // copy constructor
  //     if constexpr (Traits::hasCopyConstructor) {
  //       priv::registerClassBehaviour(m_engine, m_name, priv::ObjBehaviour::Construct,
  //                                    fmt::format("void f(const {} &in)", m_name),
  //                                    reinterpret_cast<void *>(&priv::CopyConstructor<Self>));
  //       CLOG(INFO, "AngelScript") << "\t\t+ Copy constructor";
  //     }
  //
  //     // assigment operator
  //     if constexpr (Traits::hasAssignmentOperator) {
  //       priv::registerProxyMethod(m_engine, m_name, fmt::format("{0} &opAssign(const {0} &in)", m_name),
  //                                 reinterpret_cast<void *>(&priv::assigment<Self>));
  //       CLOG(INFO, "AngelScript") << "\t\t+ Assigment operator";
  //     }
  //
  //     // destructor
  //     priv::registerClassBehaviour(m_engine, m_name, priv::ObjBehaviour::Destruct, "void f()",
  //                                  reinterpret_cast<void *>(&priv::Destructor<Self>));
  //     CLOG(INFO, "AngelScript") << "\t\t+ Destructor";
  //   }
  // };
  //
  // // Reference (AngelScript won't count or instanciate it on its own)
  // template<class T>
  // struct AsTypeBuilder<T *> {
  // private:
  //   typedef AsTypeBuilder<T *> ThisType;
  //   typedef T *Self;
  //
  //   template<class Return, class... Args>
  //   using ProxyFunction = Return (*)(Args...);
  //
  // public:
  //   explicit AsTypeBuilder(asIScriptEngine *engine)
  //     : AsTypeBuilder(std::forward<decltype(engine)>(engine), std::string(getTypeAsName<T>())) {
  //   }
  //
  //   explicit AsTypeBuilder(asIScriptEngine *engine, std::string &&name)
  //     : m_engine(std::forward<asIScriptEngine *>(engine))
  //       , m_name(std::forward<decltype(name)>(name)) {
  //     CLOG(INFO, "AngelScript") << "New AngelScript type \"" << m_name << "\" (no count)";
  //     priv::registerClassType(m_engine, m_name, 0,
  //                             static_cast<priv::ObjTypeFlags::Enum>(
  //                               priv::ObjTypeFlags::Reference | priv::ObjTypeFlags::NoCount));
  //   }
  //
  //   // Same code (AsTypeBuilder<T>)
  //   // TODO refactor later
  //
  //   // Method
  //   template<priv::Method Method>
  //   ThisType &method(std::string &&name) {
  //     const std::string declaration = getSignature<decltype(Method.ptr)>(std::forward<decltype(name)>(name));
  //     priv::registerMethod(m_engine, m_name, declaration, priv::makeDummyMethod(Method.ptr));
  //
  //     CLOG(INFO, "AngelScript") << "\t" << declaration;
  //     if constexpr (sizeof(Method.ptr) == 8) {
  //       CLOG(INFO, "AngelScript") << "\t\t at " << reinterpret_cast<void *>(Method.ptr);
  //     } else {
  //       union {
  //         decltype(&Method.ptr) mptr;
  //         void *raw[2];
  //       };
  //       CLOG(INFO, "AngelScript") << "\t\t at " << raw[0] << " " << raw[1];
  //     }
  //
  //     return *this;
  //   };
  //
  //   template<class TReturn, class... TArgs>
  //   ThisType &proxyMethod(std::string_view &&name, ProxyFunction<TReturn, T *, TArgs...> proxy) {
  //     const std::string declaration = getSignature<TReturn (*)(TArgs...)>(std::forward<decltype(name)>(name));
  //     priv::registerProxyMethod(m_engine, m_name, declaration, reinterpret_cast<void *>(proxy));
  //
  //     CLOG(INFO, "AngelScript") << "\t" << declaration;
  //     CLOG(INFO, "AngelScript") << "\t\t at " << reinterpret_cast<void *>(proxy) << " (proxy)";
  //
  //     return *this;
  //   }
  //
  //   // Property
  //
  //   template<priv::Field Field>
  //   ThisType &property(std::string_view &&name) {
  //     const std::string declaration = getPropertySignature<decltype(Field.ptr)>(std::forward<decltype(name)>(name));
  //     priv::registerProperty(m_engine, m_name, declaration, Field.getOffset());
  //
  //     CLOG(INFO, "AngelScript") << "\t" << declaration;
  //     CLOG(INFO, "AngelScript") << "\t\t with offset " << Field.getOffset();
  //     return *this;
  //   };
  //
  // private:
  //   asIScriptEngine *m_engine;
  //   std::string m_name;
  // };
} // px::script

#endif //PX_ENGINE_OBJECT_TYPE_BUILDER_HPP
