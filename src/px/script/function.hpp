// Copyright (c) 2023.

//
// Created by pyxiion on 13.12.23.
//

#ifndef PX_ENGINE_FUNCTION_HPP
#define PX_ENGINE_FUNCTION_HPP
#include "common.hpp"
#include <stdexcept>
#include <utility>

namespace px::script {
  namespace priv {
    class FunctionHandle {
    public:
      FunctionHandle(asIScriptContext *context, asIScriptFunction *function);
      ~FunctionHandle();

      FunctionHandle(const FunctionHandle &other);
      FunctionHandle &operator=(const FunctionHandle &other);

      FunctionHandle(FunctionHandle &&other) noexcept;
      FunctionHandle &operator=(FunctionHandle &&other) noexcept;

      void prepare();

      void setArg(int arg, std::int8_t   value);
      void setArg(int arg, std::int16_t  value);
      void setArg(int arg, std::int32_t  value);
      void setArg(int arg, std::int64_t  value);
      void setArg(int arg, float  value);
      void setArg(int arg, double value);

      void setArgAddress(int arg, void *ptr);

      void setArgObject(int arg, void *ptr);

      int execute();

      template<class T>
      inline T getReturn();

      void *getReturnAddress();
      void *getReturnObject();

    private:
      asIScriptContext  *m_ctx;
      asIScriptFunction *m_func;
    };
  }
  template<class TReturn, class ...TArgs>
  class Function;

  template<class TReturn, class ...TArgs>
  class Function<TReturn (TArgs...)> {
  private:
    using ThisType = Function<TReturn, TArgs...>;

    static constexpr inline bool isReturning = not std::is_same_v<TReturn, void>;

  public:
    explicit Function(priv::FunctionHandle handle)
      : m_handle(std::move(handle)) {}

    inline TReturn operator()(TArgs &&...args) {
      if constexpr (std::is_same_v<TReturn, void>) {
        call(std::forward<TArgs>(args)...);
      } else {
        return call(std::forward<TArgs>(args)...);
      }
    }

    TReturn call(TArgs &&...args) {
      m_handle.prepare();

      setArgs(std::forward<TArgs>(args)...);

      int r = m_handle.execute();
      if (r < 0) {
        throw std::runtime_error("Failed AS function execution.");
      }

      if constexpr (isReturning)
        return getReturn();
    }

  private:
    priv::FunctionHandle m_handle;

    void setArgs(TArgs &&...args) {
      int i = 0;
      ([&]{
        if constexpr (AsPrimitive<TArgs>) {
          m_handle.setArg(i, std::forward<TArgs>(args));
        } else if constexpr (AsAddress<TArgs>) {
          m_handle.setArgAddress(i, (void*)&args);
        } else { // pass copy of the arg
          m_handle.setArgObject(i, (void*)&args);
        }
        i += 1;
      }(), ...);
    };

    TReturn getReturn() {
      if constexpr (isReturning) {
        TReturn result;

        if constexpr (AsPrimitive<TReturn>) {
          // return primitive
          result = m_handle.getReturn<TReturn>();
        } else if constexpr (AsAddress<TReturn>) {
          // return a pointer or reference
          result = reinterpret_cast<TReturn>(m_handle.getReturnAddress());
        } else {
          // return a copy, since the object will be deleted after releasing the context or another function call
          result = *reinterpret_cast<TReturn *>(m_handle.getReturnObject());
        }
        return std::move(result);
      }
    }
  };

} // px::script

#endif //PX_ENGINE_FUNCTION_HPP
