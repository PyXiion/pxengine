#include "function.hpp"

px::ScriptFunction::ScriptFunction(asIScriptFunction* function)
  : m_function(function)
{
}

px::ScriptFunction::ScriptFunction(px::ScriptFunction &&other)
  : m_function(other.m_function)
{}

px::ScriptFunction::~ScriptFunction()
{
}

asIScriptFunction *px::ScriptFunction::getHandle()
{
  return m_function;
}

