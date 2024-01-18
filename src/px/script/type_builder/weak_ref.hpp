// Copyright (c) 2024.

//
// Created by pyxiion on 17.01.24.
//

#ifndef PX_SCRIPT_TYPE_BUILDER_WEAK_REF_HPP
#define PX_SCRIPT_TYPE_BUILDER_WEAK_REF_HPP
#include "common.hpp"
#include "px/px.hpp"
#include "px/memory/weak_ref.hpp"
#include "px/script/template/names.hpp"
#include "px/script/template/signatures.hpp"

namespace px::script {
  namespace priv {
    template<class T>
    bool weakRefEquals(WeakRef<T> *ptr, void *other, int typeId) {
      if (typeId != 0) {
        if (typeId & asTYPEID_OBJHANDLE) {
          other = *static_cast<void **>(other);
        }

        return ptr and *ptr == other;
      }
      return not ptr or not*ptr;
    }

    template<class, class>
    struct MethodToProxy;

    template<class TClass, class TReturn, class ...TArgs, class Handle>
    struct MethodToProxy<TReturn (TClass::*)(TArgs...), Handle> {
      using Type = TReturn (*)(Handle, TArgs...);
    };

    template<class TClass, class TReturn, class ...TArgs, class Handle>
    struct MethodToProxy<TReturn (TClass::*)(TArgs...) noexcept, Handle> {
      using Type = TReturn (*)(Handle, TArgs...) noexcept;
    };

    template<class TClass, class TReturn, class ...TArgs, class Handle>
    struct MethodToProxy<TReturn (TClass::*)(TArgs...) const, Handle> {
      using Type = TReturn (*)(Handle, TArgs...);
    };

    template<class TClass, class TReturn, class ...TArgs, class Handle>
    struct MethodToProxy<TReturn (TClass::*)(TArgs...) const noexcept, Handle> {
      using Type = TReturn (*)(Handle, TArgs...) noexcept;
    };
  }

  template<MethodPtrType auto method>
  decltype(auto) makeWeakRefProxy() {
    using Method = decltype(method);
    using T = std::remove_pointer_t<typename priv::MethodPtrToMethodProxyPtrImpl<Method>::Handle>;
    using Handle = const WeakRef<T> *;
    using ProxyPtr = typename priv::MethodToProxy<Method, Handle>::Type;

    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wreturn-type"
    ProxyPtr r = []<class ...TArgs>(Handle handle, TArgs ...args)
      -> typename function_traits<ProxyPtr>::return_type {
      if (const Ref<T> ref = handle->lock()) {
        return (*ref.*method)(std::forward<TArgs>(args) ...);
      }
      asGetActiveContext()->SetException("Calling a nullptr object.");
    };
    #pragma GCC diagnostic pop

    return r;
  }

  template<class T>
  class AsTypeBuilder<WeakRef<T>> {
  public:
    explicit AsTypeBuilder(asIScriptEngine *engine)
      : m_engine(engine)
    , m_name(getTypeAsName<T>()) {
      m_engine->RegisterObjectType(m_name.c_str(), sizeof(WeakRef<T>),
                                   asOBJ_VALUE | asOBJ_ASHANDLE | asGetTypeTraits<WeakRef<T>>());
      behaviours();
    }

    // There are no constructors or factories, since weak ref
    // cannot construct objects (it does not hold a strong reference to it)

    template<class Proxy>
      requires priv::ProxyFunctionOf<Proxy, WeakRef<T>> or priv::ProxyFunctionOf<Proxy, WeakRef<const T>>
    AsTypeBuilder &method(const std::string_view &name, Proxy *proxy) {
      constexpr bool isConst  = priv::ConstProxyFunctionOf<Proxy, T>;
      std::string declaration = getProxySignature<Proxy>(name, isConst);

      if (name.starts_with("get_") or name.starts_with("set_")) {
        // TODO: more checks for setters and getters
        declaration += " property";
      }

      const int r = m_engine->RegisterObjectMethod(m_name.c_str(), declaration.c_str(), asFUNCTION(proxy),
                                                   asCALL_CDECL_OBJFIRST);

      if (r < 0) {
        PX_THROW_AND_LOG("AngelScript", std::runtime_error,
                         "Failed to register proxy method for {} with declaration {}",
                         m_name, declaration);
      }

      CLOG(INFO, "AngelScript") << "Type " << m_name << " : " << declaration << " (proxy)";
      return *this;
    }

  private:
    void behaviours() {
      // default constructor
      int r = m_engine->RegisterObjectBehaviour(m_name.c_str(), asBEHAVE_CONSTRUCT, "void f()",
                                                asFUNCTION(&priv::Constructor<WeakRef<T>>), asCALL_CDECL_OBJFIRST);
      if (r < 0) {
        PX_THROW_AND_LOG("AngelScript", std::runtime_error, "Failed to register weakref constructor for {}", m_name);
      }

      // default destructor
      r = m_engine->RegisterObjectBehaviour(m_name.c_str(), asBEHAVE_DESTRUCT, "void f()",
                                            asFUNCTION(&priv::Destructor<WeakRef<T>>), asCALL_CDECL_OBJFIRST);
      if (r < 0) {
        PX_THROW_AND_LOG("AngelScript", std::runtime_error, "Failed to register weakref destructor for {}", m_name);
      }

      // copy constructor
      r = m_engine->RegisterObjectBehaviour(m_name.c_str(), asBEHAVE_CONSTRUCT,
                                            fmt::format("void f(const {} &in)", m_name).c_str(),
                                            asFUNCTION((&priv::Constructor<WeakRef<T>, const WeakRef<T> &>)),
                                            asCALL_CDECL_OBJFIRST);
      if (r < 0) {
        PX_THROW_AND_LOG("AngelScript", std::runtime_error, "Failed to register weakref copy constructor for {}",
                         m_name);
      }

      // handle assignment
      r = m_engine->RegisterObjectMethod(m_name.c_str(), fmt::format("{0} &opHndlAssign(const {0} &in)", m_name).c_str(),
                                         asMETHODPR(WeakRef<T>, operator=, (const WeakRef<T> &), WeakRef<T> &),
                                         asCALL_THISCALL);
      if (r < 0) {
        PX_THROW_AND_LOG("AngelScript", std::runtime_error, "Failed to register weakref handle assign operator for {}",
                         m_name);
      }

      // handle equals
      r = m_engine->RegisterObjectMethod(m_name.c_str(), fmt::format("bool opEquals(const {0} @const &in)", m_name).c_str(),
                                         asMETHODPR(WeakRef<T>, operator==, (const WeakRef<T> &) const, bool),
                                         asCALL_THISCALL);
      if (r < 0) {
        PX_THROW_AND_LOG("AngelScript", std::runtime_error, "Failed to register weakref handle equals operator for {}",
                         m_name);
      }

      // equals to null and etc
      r = m_engine->RegisterObjectMethod(m_name.c_str(), "bool opEquals(const ? &in)",
                                         asFUNCTION(&priv::weakRefEquals<T>), asCALL_CDECL_OBJFIRST);
      if (r < 0) {
        PX_THROW_AND_LOG("AngelScript", std::runtime_error, "Failed to register weakref generic equals operator for {}",
                         m_name);
      }
    }

    asIScriptEngine *m_engine;
    std::string m_name;
  };
}

#endif //PX_SCRIPT_TYPE_BUILDER_WEAK_REF_HPP
