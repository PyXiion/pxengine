#pragma once
#include "engine.hpp"

class DocumentationGenerator;

namespace px::scripts
{
  namespace priv
  {

    void bindEngine(asIScriptEngine *engine, DocumentationGenerator *docGen);

    // common
    void bindDynamicEnum(asIScriptEngine *engine, DocumentationGenerator *docGen);
    void bindMatrix4x4(asIScriptEngine *engine, DocumentationGenerator *docGen);
    void bindVector3(asIScriptEngine *engine, DocumentationGenerator *docGen);

    // graphics
    void bindRenderer(asIScriptEngine *engine, DocumentationGenerator *docGen);
    void bindShader(asIScriptEngine *engine, DocumentationGenerator *docGen);
    void bindShaderUniform(asIScriptEngine *engine, DocumentationGenerator &docGen);

    // scripts
    void bindScriptEngine(asIScriptEngine *engine, DocumentationGenerator *docGen);
    void bindScriptFunction(asIScriptEngine *engine, DocumentationGenerator *docGen);

    // system
    void bindGeneral(asIScriptEngine *engine, DocumentationGenerator *docGen);
    void bindKeycodes(asIScriptEngine *engine, DocumentationGenerator *docGen);
    void bindWindow(asIScriptEngine *engine, DocumentationGenerator *docGen);
  }

  void bindAll(ScriptEngine &engine, DocumentationGenerator *docGen);
  void bindCommon(ScriptEngine &engine, DocumentationGenerator *docGen);
  void bindGraphics(ScriptEngine &engine, DocumentationGenerator *docGen);
  void bindMath(ScriptEngine &engine, DocumentationGenerator *docGen);
  void bindAngelScript(ScriptEngine &engine, DocumentationGenerator *docGen);
  void bindSystem(ScriptEngine &engine, DocumentationGenerator *docGen);

  void prepareDocGen(asIScriptEngine *);
  void generateDoc();
}
