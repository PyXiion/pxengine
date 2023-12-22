// Copyright (c) 2023.

//
// Created by pyxiion on 13.12.23.
//

#ifndef PX_ENGINE_FUNCTION_HPP
#define PX_ENGINE_FUNCTION_HPP
#include "common.hpp"
#include <stdexcept>
#include <utility>
#include "easylogging++.h"

namespace px::script {
  namespace priv {
    class FunctionHandle {
    public:
      FunctionHandle(asIScriptContext *context, asIScriptFunction *function, asIScriptObject *self = nullptr);
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

      void setArgAddress(int arg, void **ptr);

      void setArgObject(int arg, void *ptr);

      int execute();

      template<class T>
      inline T getReturn() {
        if constexpr (not std::is_same_v<T, void>) {
          T result;

          if constexpr (AsPrimitive<T>) {
            // return primitive
            result = getReturn<T>(); // redeclared in function.cpp
          } else if constexpr (AsAddress<T>) {
            // return a pointer or reference
            result = reinterpret_cast<T>(getReturnAddress());
          } else {
            // return a copy, since the object will be deleted after releasing the context or another function call
            result = *reinterpret_cast<T *>(getReturnObject());
          }
          return std::move(result);
        }
      }

      void *getReturnAddress();
      void *getReturnObject();

      const char *getDeclaration();

      void setThis(asIScriptObject *self);

      template<class ...TArgs>
      void setArgs(TArgs ...args) {
        int i = 0;
        ([&]{
          if constexpr (AsPrimitive<TArgs>) {
            setArg(i, std::forward<TArgs>(args));
          } else if constexpr (AsAddress<TArgs>) {
            setArgAddress(i, (void**)&args);
          } else { // pass copy of the arg
            setArgObject(i, (void*)&args);
          }

          // some logging
          if constexpr (std::is_convertible_v<TArgs, std::string>) {
            CVLOG(2, "AngelScript") << "\t\tHuman readable: \"" << args << "\"";
          }
          i += 1;
        }(), ...);
      };

    private:

      asIScriptContext  *m_ctx;
      asIScriptFunction *m_func;
      asIScriptObject   *m_self;
    };
  }
  template<class TReturn, class ...TArgs>
  class Function;

  template<class TReturn, class ...TArgs>
  class Function<TReturn (TArgs...)> {
  private:
    using ThisType = Function<TReturn (TArgs...)>;

    static constexpr inline bool isReturning = not std::is_same_v<TReturn, void>;

  public:
    explicit Function(priv::FunctionHandle handle)
        : m_handle(std::move(handle)) {}

    inline TReturn operator()(TArgs ...args) {
      return call(std::forward<TArgs>(args)...);
    }

    inline TReturn call(TArgs ...args) {
      CVLOG(1, "AngelScript") << "Calling the function \"" << m_handle.getDeclaration() << "\"";
      m_handle.prepare();

      m_handle.setArgs(std::forward<TArgs>(args)...);

      int r = m_handle.execute();
      if (r < 0) {
        CLOG(ERROR, "AngelScript") << "Failed to call function.";
        throw std::runtime_error("Failed AS function execution.");
      }

      return m_handle.getReturn<TReturn>();
    }

  private:
    priv::FunctionHandle m_handle;
  };

  // Method
  template<class TReturn, class ...TArgs>
  class Method;

  template<class TReturn, class ...TArgs>
  class Method<TReturn (TArgs...)> {
  public:
    explicit Method(priv::FunctionHandle handle)
        : m_handle(std::move(handle)) {}

    inline TReturn operator()(asIScriptObject *self, TArgs ...args) {
      return call(self, std::forward<TArgs>(args)...);
    }

    inline TReturn call(asIScriptObject *self, TArgs ...args) {
      CVLOG(1, "AngelScript") << "Calling method \"" << m_handle.getDeclaration() << "\"";
      CVLOG(1, "AngelScript") << "\tSelf: " << self;

      m_handle.setThis(self);
      m_handle.prepare();

      m_handle.setArgs(std::forward<TArgs>(args)...);

      int r = m_handle.execute();
      if (r < 0) {
        CLOG(ERROR, "AngelScript") << "Failed to call method.";
        throw std::runtime_error("Failed AS method execution.");
      }

      return m_handle.getReturn<TReturn>();
    }

  private:
    priv::FunctionHandle m_handle;
  };

} // px::script

#endif //PX_ENGINE_FUNCTION_HPP
