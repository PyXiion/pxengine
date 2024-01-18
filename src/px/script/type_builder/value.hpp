// Copyright (c) 2024.

//
// Created by pyxiion on 17.01.24.
//

#ifndef PX_SCRIPT_TYPE_BUILDER_VALUE_HPP
#define PX_SCRIPT_TYPE_BUILDER_VALUE_HPP
#include <easylogging++.h>

#include "common.hpp"
#include "px/px.hpp"
#include "px/script/template/signatures.hpp"

namespace px::script {
  template<class T>
  class AsTypeBuilder {
  public:
    explicit AsTypeBuilder(asIScriptEngine *engine)
      : m_engine(engine)
    , m_name(getTypeAsName<T>()) {
      m_engine->RegisterObjectType(m_name.c_str(), sizeof(T), asOBJ_VALUE | asGetTypeTraits<T>());
    }

    template<class ...TArgs>
    void constructor() {
      const std::string declaration = fmt::format("void f({})", typesToAsTypes<TArgs ...>);

      const int r = m_engine->RegisterObjectBehaviour(m_name.c_str(), asBEHAVE_CONSTRUCT, declaration.c_str(),
                                                      asFUNCTION((&priv::Constructor<T, TArgs...>)),
                                                      asCALL_CDECL_OBJFIRST);

      if (r < 0) {
        PX_THROW_AND_LOG("AngelScript", std::runtime_error, "Failed to register constructor for {} with declaration {}",
                         m_name, declaration);
      }

      CLOG(INFO, "AngelScript") << "Type " << m_name << " : constructor " << declaration;
    }

    void destructor() {
      const int r = m_engine->RegisterObjectBehaviour(m_name.c_str(), asBEHAVE_CONSTRUCT, "",
                                                      asFUNCTION(&priv::Destructor<T>),
                                                      asCALL_CDECL_OBJFIRST);

      if (r < 0) {
        PX_THROW_AND_LOG("AngelScript", std::runtime_error, "Failed to register destructor for {}", m_name);
      }

      CLOG(INFO, "AngelScript") << "Type " << m_name << " : destructor";
    }

    template<priv::ValueFactorySignature Factory>
    void factory(Factory *factory) {
      using Args = typename priv::ValueFactorySignatureImpl<Factory>::Args;
      const std::string declaration = fmt::format("void f({})", typesToAsTypes<Args>);

      const int r = m_engine->RegisterObjectBehaviour(m_name.c_str(), asBEHAVE_CONSTRUCT, declaration.c_str(),
                                                      asFUNCTION(factory), asCALL_CDECL_OBJFIRST);

      if (r < 0) {
        PX_THROW_AND_LOG("AngelScript", std::runtime_error, "Failed to register factory for {} with declaration {}",
                         m_name, declaration);
      }

      CLOG(INFO, "AngelScript") << "Type " << m_name << " : factory " << declaration << " at " << factory;
    }

    template<MethodPtrTypeOf<T> Method>
    void method(const std::string_view &&name, Method method) {
      const std::string declaration = getSignature<Method>(name);
      const int r                   = m_engine->RegisterObjectMethod(m_name.c_str(), declaration.c_str(),
                                                   asSMethodPtr<sizeof(Method)>::Convert(method),
                                                   asCALL_THISCALL);

      if (r < 0) {
        PX_THROW_AND_LOG("AngelScript", std::runtime_error, "Failed to register method for {} with declaration {}",
                         m_name, declaration);
      }

      CLOG(INFO, "AngelScript") << "Type " << m_name << " : " << declaration;
    }

    template<priv::ProxyFunctionOf<T> Proxy>
    void method(const std::string_view &&name, Proxy *proxy) {
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
    }

    template<PropertyPtrTypeOf<T> Property>
    void property(const std::string_view &&name, Property property) {
      const std::string declaration = getPropertySignature<Property>(name);

      const int r = m_engine->RegisterObjectProperty(m_name.c_str(), declaration.c_str(), getPropertyOffset(property));

      if (r < 0) {
        PX_THROW_AND_LOG("AngelScript", std::runtime_error, "Failed to register property for {} with declaration {}",
                         m_name, declaration);
      }

      CLOG(INFO, "AngelScript") << "Type " << m_name << " : " << declaration;
    }

  private:
    asIScriptEngine *m_engine;
    std::string m_name{};
  };
}

#endif //PX_SCRIPT_TYPE_BUILDER_VALUE_HPP
