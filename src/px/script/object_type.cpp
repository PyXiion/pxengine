// Copyright (c) 2023.

//
// Created by pyxiion on 21.12.23.
//

#include "object_type.hpp"
#include <angelscript.h>

namespace px {
  namespace script {

    priv::ObjectTypeHandle::ObjectTypeHandle(asITypeInfo *typeInfo)
        : m_typeInfo(typeInfo) {

    }

    ObjectType::ObjectType(const std::string &name, asIScriptContext *ctx, asIScriptModule *module)
        : m_name(name)
        , m_type(nullptr)
        , m_ctx(ctx) {
      auto typeInfo = module->GetTypeInfoByName(name.c_str());

      if (not typeInfo) {
        CLOG(ERROR, "AngelScript") << "Failed to find type \"" << name << "\"";
        throw std::runtime_error("Failed to get AngelScript type");
      }

      m_type = priv::ObjectTypeHandle(typeInfo);
    }

    asIScriptFunction *priv::ObjectTypeHandle::getFactoryByDecl(const std::string &decl) {
      return m_typeInfo->GetFactoryByDecl(decl.c_str());
    }

    asIScriptFunction *priv::ObjectTypeHandle::getMethodByDecl(const std::string &decl) {
      return m_typeInfo->GetMethodByDecl(decl.c_str());
    }

    uint priv::ObjectTypeHandle::getPropertyByDecl(const std::string &decl) {
      CVLOG(2, "AngelScript") << "Looking for " << decl << " property";
      uint propertyCount = m_typeInfo->GetPropertyCount();
      CVLOG(2, "AngelScript") << "Found " << propertyCount << " properties in the type:";
      for (uint i = 0; i < propertyCount; i++) {
        const char *cdecl = m_typeInfo->GetPropertyDeclaration(i);
        bool match = decl == cdecl;

        CVLOG(2, "AngelScript") << "\t" << i << ": " << cdecl << (match ? " matched" : "");
        if (match) {
          return i;
        }
      }

      CVLOG(2, "AngelScript") << "Property not found.";
      asIScriptObject *obj;
      return UINT_MAX;
    }

    void *priv::ObjectTypeHandle::getPropertyAddress(asIScriptObject *obj, uint property) {
      return obj->GetAddressOfProperty(property);
    }

    bool priv::ObjectTypeHandle::derivesFrom(const ObjectTypeHandle &base) const {
      if (m_typeInfo and base.m_typeInfo) {
        return m_typeInfo->DerivesFrom(base.m_typeInfo);
      }
      return false;
    }

    bool ObjectType::derivesFrom(const ObjectType &base) const {
      return m_type.derivesFrom(base.m_type);
    }
  } // px
} // script