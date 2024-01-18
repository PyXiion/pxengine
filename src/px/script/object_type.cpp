// Copyright (c) 2023.

//
// Created by pyxiion on 21.12.23.
//

#include <angelscript.h>
#include "object_type.hpp"

namespace px {
  namespace script {
    ObjectType::ObjectTypeHandle::ObjectTypeHandle(asITypeInfo *typeInfo)
      : m_typeInfo(typeInfo) {}

    ObjectType::ObjectType(const std::string &name, asIScriptContext *ctx, asIScriptModule *module)
      : m_name(name)
    , m_type(nullptr)
    , m_ctx(ctx) {
      auto typeInfo = module->GetTypeInfoByName(name.c_str());

      if (not typeInfo) {
        CLOG(ERROR, "AngelScript") << "Failed to find type \"" << name << "\"";
        throw std::runtime_error("Failed to get AngelScript type");
      }

      m_type = ObjectTypeHandle(typeInfo);
    }

    ObjectType::ObjectType(const unsigned index, asIScriptContext *ctx, const asIScriptModule *module)
      : m_ctx(ctx)
    , m_type(nullptr) {
      auto typeInfo = module->GetObjectTypeByIndex(index);

      if (not typeInfo) {
        CLOG(ERROR, "AngelScript") << "Failed to find type by index " << index;
        throw std::runtime_error("Failed to get AngelScript type");
      }

      m_name = typeInfo->GetName();
      m_type = ObjectTypeHandle(typeInfo);
    }

    asIScriptFunction *ObjectType::ObjectTypeHandle::getFactoryByDecl(const std::string &decl) const {
      return m_typeInfo->GetFactoryByDecl(decl.c_str());
    }

    asIScriptFunction *ObjectType::ObjectTypeHandle::getMethodByDecl(const std::string &decl) const {
      return m_typeInfo->GetMethodByDecl(decl.c_str());
    }

    uint ObjectType::ObjectTypeHandle::getPropertyByDecl(const std::string &decl) const {
      CVLOG(2, "AngelScript") << "Looking for " << decl << " property";
      uint propertyCount = m_typeInfo->GetPropertyCount();
      CVLOG(2, "AngelScript") << "Found " << propertyCount << " properties in the type:";
      for (uint i = 0; i < propertyCount; i++) {
        const char *cdecl = m_typeInfo->GetPropertyDeclaration(i);
        bool match        = decl == cdecl;

        CVLOG(2, "AngelScript") << "\t" << i << ": " << cdecl << (match ? " matched" : "");
        if (match) {
          return i;
        }
      }

      CVLOG(2, "AngelScript") << "Property not found.";
      asIScriptObject *obj;
      return UINT_MAX;
    }

    void *ObjectType::ObjectTypeHandle::getPropertyAddress(asIScriptObject *obj, const uint property) {
      return obj->GetAddressOfProperty(property);
    }

    bool ObjectType::ObjectTypeHandle::derivesFrom(const ObjectTypeHandle &base) const {
      if (m_typeInfo and base.m_typeInfo) {
        return m_typeInfo->DerivesFrom(base.m_typeInfo);
      }
      return false;
    }

    bool ObjectType::derivesFrom(const ObjectType &base) const {
      return m_type.derivesFrom(base.m_type);
    }

    std::string_view ObjectType::getName() const {
      return m_type.m_typeInfo->GetName();
    }
  } // px
}   // script
