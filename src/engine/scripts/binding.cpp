#include "binding.hpp"
#include <memory>
#include <cassert>
#include <angelscript.h>
#include "docgen.hpp"
#include "../engine.hpp"
#include "../common/dynamic_enum.hpp"
#include "../common/matrix4x4.hpp"

#define CHECK assert(r >= 0);

#define DOC_TYPE(desc) r = docGen->DocumentObjectType(r, desc); CHECK
#define DOC_BEHAVIOUR(desc) r = docGen->DocumentObjectBehaviour(r, desc); CHECK
#define DOC_METHOD(desc) r = docGen->DocumentObjectMethod(r, desc); CHECK

template<class T, class ...TArgs>
void TConstructor(void *memory, TArgs ...params)
{
  new(memory) T(params...);
}
template<class T>
void TDestructor(void *memory)
{
  static_cast<T*>(memory)->~T();
}

template<class T>
void TAssign(T &self, T& other)
{
  self = other;
}

void px::scripts::bindAll(px::ScriptEngine &engine, DocumentationGenerator *docGen)
{
  // priv::bindEngine(engine.getHandle(), docGen);
  bindCommon(engine, docGen);
}


void px::scripts::bindCommon(px::ScriptEngine &_engine, DocumentationGenerator *docGen)
{
  asIScriptEngine *engine = _engine.getHandle();
  priv::bindDynamicEnum(engine, docGen);
  priv::bindVector3(engine, docGen);
  priv::bindMatrix4x4(engine, docGen);
}

void px::scripts::bindAngelScript(px::ScriptEngine &_engine, DocumentationGenerator *docGen)
{
  asIScriptEngine *engine = _engine.getHandle();

  priv::bindScriptEngine(engine, docGen);
}

void px::scripts::priv::bindEngine(asIScriptEngine *engine, DocumentationGenerator *docGen)
{
  int r;
  r = engine->RegisterObjectType("Engine", 0, asOBJ_REF | asOBJ_NOHANDLE); CHECK
  DOC_TYPE("Самый главный класс. Представляет собой движок и всё, что с ним связано. Буквально всё.")

  r = engine->RegisterObjectMethod("Engine", "Window &get_window() property", asMETHOD(Engine, getWindow), asCALL_THISCALL);
  DOC_METHOD("Текущее окно.")

  r = engine->RegisterObjectMethod("Engine", "Renderer &get_renderer() property", asMETHOD(Engine, getRenderer), asCALL_THISCALL);
  DOC_METHOD("Текущий отрисовщик.")

  r = engine->RegisterObjectMethod("Engine", "ScriptEngine &get_scriptEngine() property", asMETHOD(Engine, getScriptEngine), asCALL_THISCALL);
  DOC_METHOD("Текущий скриптовой движок.")
}

// COMMON
void px::scripts::priv::bindDynamicEnum(asIScriptEngine *engine, DocumentationGenerator *docGen)
{
  int r;
  r = engine->RegisterObjectType("DynamicEnum", sizeof(DynamicEnum), asOBJ_VALUE | asGetTypeTraits<DynamicEnum>()); CHECK
  DOC_TYPE("Класс, который просто является двойным std::unordered_map<int, string> и <string, int>")

  r = engine->RegisterObjectBehaviour("DynamicEnum", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(TConstructor<DynamicEnum>), asCALL_CDECL_OBJLAST); CHECK
  r = engine->RegisterObjectBehaviour("DynamicEnum", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(TDestructor<DynamicEnum>), asCALL_CDECL_OBJLAST); CHECK

  r = engine->RegisterObjectMethod("DynamicEnum", "DynamicEnum &opAssign(const DynamicEnum &in)", asFUNCTION(TAssign<DynamicEnum>), asCALL_CDECL_OBJFIRST); CHECK

  r = engine->RegisterObjectMethod("DynamicEnum", "int add(const string &in)", asMETHOD(DynamicEnum, add), asCALL_THISCALL); CHECK
  DOC_METHOD("Добавить одно enum значение. Возвращает его целочисленный ID.")

  r = engine->RegisterObjectMethod("DynamicEnum", "int get(const string &in) const", asMETHODPR(DynamicEnum, get, (const std::string&) const, DynamicEnum::EnumValue), asCALL_THISCALL); CHECK
  DOC_METHOD("Получить целочисленный ID по строковому ID.")

  r = engine->RegisterObjectMethod("DynamicEnum", "string get(int) const", asMETHODPR(DynamicEnum, get, (DynamicEnum::EnumValue) const, std::string), asCALL_THISCALL); CHECK
  DOC_METHOD("Получить строковой ID по целочисленному.")
}
void px::scripts::priv::bindMatrix4x4(asIScriptEngine *engine, DocumentationGenerator *docGen)
{
  int r;
  r = engine->RegisterObjectType("Matrix4x4", sizeof(Matrix4x4), asOBJ_VALUE | asOBJ_POD | asGetTypeTraits<Matrix4x4>()); CHECK
  DOC_TYPE("Матрица 4 на 4.")

  r = engine->RegisterObjectBehaviour("Matrix4x4", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(TConstructor<Matrix4x4>), asCALL_CDECL_OBJLAST); CHECK
  r = engine->RegisterObjectBehaviour("Matrix4x4", asBEHAVE_LIST_CONSTRUCT,
                                      "void f(int &in) {float, float, float, float,"
                                                       "float, float, float, float,"
                                                       "float, float, float, float,"
                                                       "float, float, float, float}"
                                      , asFUNCTION((TConstructor<Matrix4x4, float*>)), asCALL_CDECL_OBJFIRST); CHECK
  // no need in destructor
  // r = engine->RegisterObjectBehaviour("Matrix4x4", asBEHAVE_DESTRUCT, "void f()", asFUNCTION(TDestructor<Matrix4x4>), asCALL_CDECL_OBJLAST); CHECK

  r = engine->RegisterObjectMethod("Matrix4x4", "Matrix4x4 &opAssign(const Matrix4x4 &in)", asFUNCTION(TAssign<Matrix4x4>), asCALL_CDECL_OBJFIRST); CHECK

  r = engine->RegisterObjectMethod("Matrix4x4", "Matrix4x4 opMul(const Matrix4x4 &in) const", asMETHODPR(Matrix4x4, operator*, (const Matrix4x4&) const, Matrix4x4), asCALL_THISCALL);
  DOC_METHOD("Умножение матрицы на матрицу.")

  r = engine->RegisterObjectMethod("Matrix4x4", "Vector3 opMul(const Vector3 &in) const", asMETHODPR(Matrix4x4, operator*, (const Vector3&) const, Vector3), asCALL_THISCALL);
  DOC_METHOD("Умножение матрицы на вектор.")

  r = engine->RegisterObjectMethod("Matrix4x4", "float get(int j, int k) const", asMETHOD(Matrix4x4, get), asCALL_THISCALL); CHECK
  DOC_METHOD("Получить элемент матрицы.")

  r = engine->RegisterObjectMethod("Matrix4x4", "void set(int j, int k, float value)", asMETHOD(Matrix4x4, set), asCALL_THISCALL); CHECK;
  DOC_METHOD("Установить элемент матрицы.")
}
void px::scripts::priv::bindVector3(asIScriptEngine *engine, DocumentationGenerator *docGen)
{
  int r;
  r = engine->RegisterObjectType("Vector3", sizeof(Vector3), asOBJ_VALUE | asOBJ_POD | asOBJ_APP_CLASS_ALLFLOATS | asGetTypeTraits<Vector3>()); CHECK
  DOC_TYPE("3D вектор.")

  r = engine->RegisterObjectBehaviour("Vector3", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(TConstructor<Vector3>), asCALL_CDECL_OBJFIRST);
  r = engine->RegisterObjectBehaviour("Vector3", asBEHAVE_CONSTRUCT, "void f(float x, float y, float z)", asFUNCTION((TConstructor<Vector3, float, float, float>)), asCALL_CDECL_OBJFIRST);

  r = engine->RegisterObjectMethod("Vector3", "float get_length() property", asMETHOD(Vector3, length), asCALL_THISCALL);
  DOC_METHOD("Длина вектора.")

  r = engine->RegisterObjectMethod("Vector3", "float dot(const Vector3 &in)", asMETHOD(Vector3, dot), asCALL_THISCALL);
  DOC_METHOD("Возвращает результат скалярного произведения с другим вектором.")

  r = engine->RegisterObjectMethod("Vector3", "Vector3 cross(const Vector3 &in)", asMETHOD(Vector3, cross), asCALL_THISCALL);
  DOC_METHOD("Возвращает результат не помню как по-русски")

  r = engine->RegisterObjectMethod("Vector3", "Vector3 opEquals(const Vector3 &in) const", asMETHOD(Vector3, operator==), asCALL_THISCALL);
  DOC_METHOD("Сравнить два вектора.")

  r = engine->RegisterObjectMethod("Vector3", "Vector3 opNeg() const", asMETHODPR(Vector3, operator-, () const, Vector3), asCALL_THISCALL);
  DOC_METHOD("Поменять знаки в элементах вектора.")

  r = engine->RegisterObjectMethod("Vector3", "Vector3 opSub(const Vector3 &in) const", asMETHODPR(Vector3, operator-, (const Vector3&) const, Vector3), asCALL_THISCALL);
  DOC_METHOD("Вычесть один вектор из другого.")

  r = engine->RegisterObjectMethod("Vector3", "Vector3 opAdd(const Vector3 &in) const", asMETHODPR(Vector3, operator+, (const Vector3&) const, Vector3), asCALL_THISCALL);
  DOC_METHOD("Сложить два вектора.")

  r = engine->RegisterObjectMethod("Vector3", "Vector3 opMul(float) const", asMETHOD(Vector3, operator*), asCALL_THISCALL);
  DOC_METHOD("Умножить вектор на скаляр.")

  r = engine->RegisterObjectMethod("Vector3", "Vector3 opMul_r(float) const", asMETHOD(Vector3, operator*), asCALL_THISCALL);

  r = engine->RegisterObjectMethod("Vector3", "Vector3 opDiv(float) const", asMETHOD(Vector3, operator/), asCALL_THISCALL);
  DOC_METHOD("Разделить вектор на скаляр.")


  r = engine->RegisterObjectMethod("Vector3", "Vector3 &opAssign(const Vector3 &in)", asFUNCTION(TAssign<Vector3>), asCALL_CDECL_OBJFIRST);
  DOC_METHOD("Копирует вектор.")

  r = engine->RegisterObjectMethod("Vector3", "Vector3 &opAddAssign(const Vector3 &in)", asMETHOD(Vector3, operator+=), asCALL_THISCALL);
  r = engine->RegisterObjectMethod("Vector3", "Vector3 &opSubAssign(const Vector3 &in)", asMETHOD(Vector3, operator-=), asCALL_THISCALL);

  r = engine->RegisterObjectMethod("Vector3", "Vector3 &opMulAssign(float)", asMETHOD(Vector3, operator*=), asCALL_THISCALL);
  r = engine->RegisterObjectMethod("Vector3", "Vector3 &opDivAssign(float)", asMETHOD(Vector3, operator/=), asCALL_THISCALL);


  r = engine->RegisterObjectProperty("Vector3", "float x", asOFFSET(Vector3, x));
  r = engine->RegisterObjectProperty("Vector3", "float y", asOFFSET(Vector3, y));
  r = engine->RegisterObjectProperty("Vector3", "float z", asOFFSET(Vector3, z));
}


// SCRIPTS

void px::scripts::priv::bindScriptEngine(asIScriptEngine *engine, DocumentationGenerator *docGen)
{
  int r;
  r = engine->RegisterObjectType("ScriptEngine", 0, asOBJ_REF | asOBJ_NOHANDLE); CHECK
  DOC_TYPE("Управляет скриптами AngelScript.")

  r = engine->RegisterObjectMethod("ScriptEngine", "ScriptModule getOrCreateModule(const string name &in)", asMETHOD(ScriptEngine, getOrCreateModule), asCALL_THISCALL); CHECK
  DOC_METHOD("Получить или создать новый модуль по его имени.")

}

void ScriptFunction_getHandle(px::ScriptFunction &self, void *ptr)
{
  ptr = self.getHandle();
}

void px::scripts::priv::bindScriptFunction(asIScriptEngine *engine, DocumentationGenerator *docGen)
{
  int r;
  r = engine->RegisterObjectType("ScriptFunction", 0, asOBJ_VALUE | asOBJ_POD); CHECK
  DOC_TYPE("Функция AngelScript")


  r = engine->RegisterObjectMethod("ScriptFunction", "void getHandle(?&inout)", asFUNCTION(ScriptFunction_getHandle), asCALL_GENERIC); CHECK
  DOC_METHOD("Получить указатель на asIScriptFunction.")
}
