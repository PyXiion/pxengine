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

      int release();

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

  public:
    explicit Function(priv::FunctionHandle handle)
      : m_handle(std::move(handle)) {}

    inline TReturn operator()(TArgs ...args) {
      if constexpr (std::is_same_v<TReturn, void>) {
        call(std::forward<TArgs>(args)...);
      } else {
        return call(std::forward<TArgs>(args)...);
      }
    }

    TReturn call(TArgs ...args) {
      m_handle.prepare();

      // pass args
      int i = 0;
      ([&]{
        m_handle.setArg(i, args);
        i += 1;
      }(), ...);

      int r = m_handle.execute();
      if (r < 0) {
        throw std::runtime_error("Failed AS function execution.");
      }

      if constexpr (not std::is_same_v<TReturn, void>) {
        TReturn result;

        if constexpr (AsPrimitive<TReturn>) {
          // return primitive
          result = m_handle.getReturn<TReturn>();
        } else if constexpr (AsAddress<TReturn>) {
          // TODO comments
          result = reinterpret_cast<TReturn>(m_handle.getReturnAddress());
        } else {
          result = *reinterpret_cast<TReturn *>(m_handle.getReturnObject());
        }
        m_handle.release();
        return std::move(result);
      } else {
        m_handle.release();
        return;
      }
    }

  private:
    priv::FunctionHandle m_handle;
  };

} // px::script

#endif //PX_ENGINE_FUNCTION_HPP
