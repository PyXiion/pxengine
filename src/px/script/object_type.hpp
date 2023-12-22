// Copyright (c) 2023.

//
// Created by pyxiion on 21.12.23.
//

#ifndef PX_ENGINE_OBJECT_TYPE_HPP
#define PX_ENGINE_OBJECT_TYPE_HPP
#include "common.hpp"
#include "function.hpp"
#include "template/signatures.hpp"

namespace px {
  namespace script {
    namespace priv {
      class ObjectTypeHandle {
      public:
        explicit ObjectTypeHandle(asITypeInfo *typeInfo);

        asIScriptFunction *getFactoryByDecl(const std::string &decl);
        asIScriptFunction *getMethodByDecl(const std::string &decl);
        uint getPropertyByDecl(const std::string &decl);

        static void *getPropertyAddress(asIScriptObject *obj, uint property);

      private:
        asITypeInfo *m_typeInfo;
      };
    }

    class ObjectType {
    public:
      ObjectType(const std::string &name, asIScriptContext *ctx, asIScriptModule *module);

      template<class ...TArgs>
      auto getFactory() {
        const std::string decl = getFactorySignature<TArgs...>(m_name.c_str());

        asIScriptFunction *funcHandle = m_type.getFactoryByDecl(decl);
        if (not funcHandle) {
          CLOG(ERROR, "AngelScript") << "Failed to get a factory by declaration " << decl;
          throw std::runtime_error(fmt::format("Failed to get factory by declaration {}.", decl));
        }
        CVLOG(1, "AngelScript") << "Requested AngelScript factory by declaration " << decl;

        return Function<asIScriptObject **(TArgs...)>(priv::FunctionHandle(m_ctx, funcHandle));
      };

      template<class T, class ...TArgs>
      auto getMethod(const std::string &name) {
        const std::string decl = getSignature<T (TArgs...)>(name);
        return getMethodByDecl<T, TArgs...>(decl);
      }
      template<class T, class ...TArgs>
      auto getMethodByDecl(const std::string &decl) {
        asIScriptFunction *funcHandle = m_type.getMethodByDecl(decl);
        // if not found, try get const
        if (not funcHandle)
          funcHandle = m_type.getMethodByDecl(decl + " const");

        if (not funcHandle) {
          CLOG(ERROR, "AngelScript") << "Failed to get a method by declaration " << decl;
          throw std::runtime_error(fmt::format("Failed to get method by declaration {}.", decl));
        }
        CVLOG(1, "AngelScript") << "Requested AngelScript method by declaration " << decl;

        using Signature = T (typename priv::GetTypeAsNameImpl<TArgs>::Type...);
        return Method<Signature>(priv::FunctionHandle(m_ctx, funcHandle));
      }

      template<class T>
      auto getProperty(const std::string &name) {
        auto typeName = getTypeAsName<T>();
        std::string signature = fmt::format("{} {}", typeName, name);

        auto propertyId = m_type.getPropertyByDecl(signature);

        if (propertyId != UINT_MAX) {
          return [propertyId](asIScriptObject *self) -> T& {
            void *address = priv::ObjectTypeHandle::getPropertyAddress(self, propertyId);
            return *reinterpret_cast<T*>(address);
          };
        };
        CLOG(ERROR, "AngelScript") << "Failed to get property " << signature;
        throw std::runtime_error("Failed to get property " + signature);
      }

    private:
      std::string m_name;
      asIScriptContext *m_ctx;
      priv::ObjectTypeHandle m_type;
    };

  } // px
} // script

#endif //PX_ENGINE_OBJECT_TYPE_HPP
