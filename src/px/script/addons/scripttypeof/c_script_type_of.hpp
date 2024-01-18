//
// Created by pyxiion on 14.01.24.
//

#ifndef C_SCRIPT_TYPE_OF_HPP
#define C_SCRIPT_TYPE_OF_HPP
#include <angelscript.h>

class CScriptTypeOf {
public:
  explicit CScriptTypeOf(int typeId);

  const int typeId;
};

void RegisterTypeOf(asIScriptEngine *engine);

#endif //C_SCRIPT_TYPE_OF_HPP
