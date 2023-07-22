#include "module.hpp"
#include <fstream>
#include <stdexcept>
#include <cassert>

#include <fmt/format.h>

#include "engine.hpp"

px::ScriptModule::ScriptModule(px::ScriptEngine &engine, std::string module_name)
  : m_name(module_name)
  , m_engine(engine)
{
  m_handle = engine.getHandle()->GetModule(m_name.c_str(), asGM_CREATE_IF_NOT_EXISTS);
  m_builder.Set(engine.getHandle(), m_handle);
}

void px::ScriptModule::loadScriptFromFile(const std::string &filename)
{
  int r = m_builder.AddSectionFromFile(filename.c_str()); assert(r >= 0);
}

void px::ScriptModule::loadScriptFromString(const std::string &source_name, const std::string &code)
{
  int r = m_builder.AddSectionFromMemory(source_name.c_str(), code.c_str()); assert(r >= 0);
}

void px::ScriptModule::build()
{
  int r = m_builder.BuildModule(); assert(r >= 0);
  // TODO check is compiled
}


px::ScriptFunction px::ScriptModule::getFunctionByDecl(const std::string& function_decl)
{
  asIScriptFunction *func = m_handle->GetFunctionByDecl(function_decl.c_str());
  if (func == nullptr)
    throw std::runtime_error(fmt::format("Не удалось найти {} в модуле {}", function_decl, m_name));
  return ScriptFunction(func);
}
px::ScriptFunction px::ScriptModule::getFunctionByName(const std::string& function_name)
{
  asIScriptFunction *func = m_handle->GetFunctionByName(function_name.c_str());
  if (func == nullptr)
    throw std::runtime_error(fmt::format("Не удалось найти {} в модуле {}", function_name, m_name));
  return ScriptFunction(func);
}



asIScriptModule *px::ScriptModule::getHandle()
{
  return m_handle;
}
px::ScriptEngine & px::ScriptModule::getEngine()
{
  return m_engine;
}
