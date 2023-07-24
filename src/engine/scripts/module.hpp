#pragma once
#include <string>
#include <memory>

#include <angelscript.h>

#include "addons/scriptbuilder.h"
#include "function.hpp"

namespace px
{
  class ScriptEngine;
  class ScriptModule
  {
  public:
    ScriptModule();
    ScriptModule(ScriptEngine &engine, std::string module_name);

    void loadScriptFromFile(const std::string &filename);
    void loadScriptFromString(const std::string &source_name, const std::string &code);

    void build();

    ScriptFunction getFunctionByName(const std::string &function_name);
    ScriptFunction getFunctionByDecl(const std::string &function_decl);

    asIScriptModule *getHandle();
    ScriptEngine *getEngine();

  private:
    std::string m_name;
    ScriptEngine *m_engine;
    CScriptBuilder m_builder;

    asIScriptModule *m_handle;

    void check() const;
  };
}
