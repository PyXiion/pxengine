#include "engine.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <cassert>

#include <yaml-cpp/yaml.h>
#include <fmt/format.h>

#include "addons/scriptstdstring.h"
#include "binding.hpp"
#include "docgen.hpp"

namespace fs = std::filesystem;

px::ScriptEngine::ScriptEngine()
{
  initEngine();
}

px::ScriptEngine::~ScriptEngine()
{
}

px::ScriptModule px::ScriptEngine::getOrCreateModule(const std::string &module_name)
{
  return ScriptModule(*this, module_name);
}


void px::ScriptEngine::bindAll()
{
  ScriptDocumentationOptions options;

  options.projectName = "PyXiion's engine";
  options.includeArrayInterface = false;
  options.includeStringInterface = false;
  options.includeWeakRefInterface = false;
  options.htmlSafe = false;

  DocumentationGenerator docGen(getHandle(), options);

  scripts::bindAll(*this, &docGen);

  docGen.Generate();
}


asIScriptEngine *px::ScriptEngine::getHandle()
{
  return m_script_engine;
}


void px::ScriptEngine::initEngine()
{
  m_script_engine = asCreateScriptEngine();

  int r = m_script_engine->SetMessageCallback(asFUNCTION(&messageCallback), this, asCALL_CDECL); assert(r >= 0);
  // r = m_script_engine->SetContextCallbacks(asFUNCTION(&exceptionCallback), this, asCALL_CDECL); assert(r >= 0);

  RegisterStdString(m_script_engine);

  r = m_script_engine->RegisterGlobalFunction("void print(const string &in)", asFUNCTION(print), asCALL_CDECL); assert(r >= 0);
  r = m_script_engine->RegisterGlobalFunction("void println(const string &in)", asFUNCTION(println), asCALL_CDECL); assert(r >= 0);
}


void px::ScriptEngine::print(const std::string &value)
{
  std::cout << value;
}

void px::ScriptEngine::println(const std::string &value)
{
  std::cout << value << std::endl;
}

void px::ScriptEngine::messageCallback(const asSMessageInfo *msg, void *param)
{
  std::string level = "Ошибка";
  if (msg->type == asMSGTYPE_WARNING)
    level = "Предупреждение";
  else if (msg->type == asMSGTYPE_INFORMATION)
    level = "Информация";

  std::cout << fmt::format("[AngelScript] [{}] {}:{}:{} {}", level, msg->section, msg->row, msg->col, msg->message) << std::endl;
}

void px::ScriptEngine::exceptionCallback(asIScriptContext *ctx)
{
  // Determine the exception that occurred
  printf("Описание:\t%s\n", ctx->GetExceptionString());

  // Determine the function where the exception occurred
  const asIScriptFunction *function = ctx->GetExceptionFunction();
  printf("Функция:\t%s\n", function->GetDeclaration());
  printf("Модуль:\t%s\n", function->GetModuleName());
  printf("Файл:\t%s\n", function->GetScriptSectionName());

  // Determine the line number where the exception occurred
  printf("Строка:\t%d\n", ctx->GetExceptionLineNumber());
}
