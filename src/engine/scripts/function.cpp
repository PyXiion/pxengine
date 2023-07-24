#include "function.hpp"

px::ScriptFunction::ScriptFunction(asIScriptFunction* function)
  : m_function(function)
{
  if (m_function)
    m_function->AddRef();
}

px::ScriptFunction::ScriptFunction(const ScriptFunction &other)
  : m_function(other.m_function)
{
  if (m_function)
    m_function->AddRef();
}

px::ScriptFunction::ScriptFunction(px::ScriptFunction &&other)
  : m_function(other.m_function)
{
}

px::ScriptFunction::~ScriptFunction()
{
  if (m_function)
    m_function->Release();
}

asIScriptFunction *px::ScriptFunction::getHandle()
{
  return m_function;
}

void px::ScriptFunction::check()
{
  if (m_function == nullptr)
    throw std::runtime_error("px::ScriptFunction: m_function == nullptr");
}
