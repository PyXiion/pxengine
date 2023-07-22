#pragma once
#include <string>
#include <memory>
#include <angelscript.h>

#include "module.hpp"

namespace px
{
  class ScriptEngine
  {
  public:
    ScriptEngine();
    ~ScriptEngine();

    ScriptModule getOrCreateModule(const std::string &module_name);

    void bindAll();

    asIScriptEngine *getHandle();

  private:
    asIScriptEngine *m_script_engine;

    void initEngine();

    static void messageCallback(const asSMessageInfo *msg, void *param);
    static void exceptionCallback(asIScriptContext *ctx);

    static void print(const std::string &value);
    static void println(const std::string &value);
  };
}
