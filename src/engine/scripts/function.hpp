#pragma once
#include <iostream>
#include <type_traits>
#include <cassert>
#include <angelscript.h>

namespace px
{
  template<class T>
  struct toRefArg
  {
    toRefArg(T &v) : v(v) {}

    T &v;
  };

  template<typename>
  struct is_ref_arg : std::false_type {};

  template<class T>
  struct is_ref_arg<toRefArg<T>> : std::true_type {};

  template<class T>
  inline constexpr auto is_ref_arg_v = is_ref_arg<T>::value;

  class ScriptFunction
  {
  public:
    ScriptFunction(asIScriptFunction *function = nullptr);
    ScriptFunction(const ScriptFunction &other);
    ScriptFunction(ScriptFunction &&other);

    ~ScriptFunction();

    /**
     * @brief Вызвать функцию AngelScript с указанными параметрами.
     * Вызывает функцию или метод AngelScript с указанными шаблонными параметрами.
     * Для передачи объекта-ссылки (не handle), нужно использовать @ref toRefArg.
     * 
     * Пример:
     * @code
     * SomeObjValueType value = 34;
     * function.call(px::toRefArg(value));
     * // или
     * function.call<void, SomeObjValueType&>(value); // то же самое
     * @endcode
     * Если так не сделать, то функция получит только копию этого объекта.
     */
    template<typename TRet, typename ...TArgs>
    TRet call(TArgs ...args);

    template<typename TRet = void, typename ...TArgs>
    TRet operator()(TArgs ...args)
    {
      return call<TRet>(args...);
    }

    asIScriptFunction *getHandle();

    ScriptFunction &operator=(const ScriptFunction &) = default;

  private:
    asIScriptFunction *m_function;

    void check();
  };
}

template<typename TRet = void, typename ...TArgs>
TRet px::ScriptFunction::call(TArgs... args)
{
  asIScriptEngine *engine = m_function->GetEngine();
  asIScriptContext *ctx = engine->CreateContext();

  ctx->Prepare(m_function);

  int i = 0;
  // Iterate all args
  ([&]
  {
    if constexpr (std::is_same_v<int8_t, decltype(args)>) // arg is byte
    {
      ctx->SetArgByte(i, args);
    }
    else if constexpr (std::is_same_v<int16_t, decltype(args)>) // arg is word
    {
      ctx->SetArgWord(i, args);
    }
    else if constexpr (std::is_same_v<int32_t, decltype(args)>) // arg is dword
    {
      ctx->SetArgDWord(i, args);
    }
    else if constexpr (std::is_same_v<int64_t, decltype(args)>) // arg is qword
    {
      ctx->SetArgQWord(i, args);
    }
    else if constexpr (std::is_same_v<float, decltype(args)>) // arg is float
    {
      ctx->SetArgFloat(i, args);
    }
    else if constexpr (std::is_same_v<double, decltype(args)>) // arg is double
    {
      ctx->SetArgDouble(i, args);
    }
    else if constexpr (std::is_same_v<std::string, decltype(args)>)
    {
      ctx->SetArgObject(i, &args);
    }
    else if constexpr (std::is_pointer_v<decltype(args)>) // pass as pointer
    {
      ctx->SetArgAddress(i, reinterpret_cast<void *>(args));
    }
    else if constexpr (is_ref_arg_v<decltype(args)>) // pass as object
    {
      ctx->SetArgObject(i, &args.v);
    }
    else
    {
      ctx->SetArgObject(i, &args); // passed as copy of object
    }
    i += 1;
  }(), ...);

  int r = ctx->Execute();

  if (r == asEXECUTION_EXCEPTION)
  {
    throw 0; // TODO
  }
  else if (r == asEXECUTION_FINISHED)
  {
    if constexpr (std::is_same_v<TRet, void>)
    {
      // nothing to return
      ctx->Release();
    }
    else
    {
      std::remove_reference_t<TRet> *returnValue = nullptr;
      if constexpr (std::is_integral_v<TRet> || std::is_floating_point_v<TRet>)
      {
        if constexpr (std::is_same_v<TRet, int8_t> || std::is_same_v<TRet, uint8_t>)
        {
          returnValue = new TRet;
          *returnValue = ctx->GetReturnByte();
        }
        else if constexpr (std::is_same_v<TRet, int16_t> || std::is_same_v<TRet, uint16_t>)
        {
          returnValue = new TRet;
          *returnValue = ctx->GetReturnWord();
        }
        else if constexpr (std::is_same_v<TRet, int32_t> || std::is_same_v<TRet, uint32_t>)
        {
          returnValue = new TRet;
          *returnValue = ctx->GetReturnDWord();
        }
        else if constexpr (std::is_same_v<TRet, int64_t> || std::is_same_v<TRet, uint64_t>)
        {
          returnValue = new TRet;
          *returnValue = ctx->GetReturnQWord();
        }
        else if constexpr (std::is_same_v<TRet, float>)
        {
          returnValue = new TRet;
          *returnValue = ctx->GetReturnFloat();
        }
        else if constexpr (std::is_same_v<TRet, double>)
        {
          returnValue = new TRet;
          *returnValue = ctx->GetReturnDouble();
        }
      }
      else if constexpr (std::is_pointer_v<TRet>)
      {
        returnValue = static_cast<TRet>(ctx->GetReturnAddress());
      }
      else if constexpr (std::is_reference_v<TRet>)
      {
        returnValue = static_cast<std::remove_reference_t<TRet>*>(ctx->GetReturnAddress());
      }
      else
      {
        TRet returnCopy(*static_cast<TRet*>(ctx->GetReturnObject()));
        ctx->Release();
        return returnCopy;
      }

      ctx->Release();
      if constexpr (std::is_reference_v<TRet>)
        return *returnValue;
      else if constexpr (std::is_pointer_v<TRet>)
        return returnValue;
    }
  }
  else
  {
    throw 0; // TODO
  }
}
