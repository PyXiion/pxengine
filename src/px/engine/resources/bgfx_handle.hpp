// Copyright (c) 2023.

//
// Created by pyxiion on 06.11.23.
//

#ifndef ENGINE_BGFX_HANDLE_HPP
#define ENGINE_BGFX_HANDLE_HPP
#include <bgfx/bgfx.h>
#include <easy/profiler.h>

#include "px/px.hpp"

namespace px {

  template<class T>
  class BgfxUniqueHandle {
  public:
    BgfxUniqueHandle(T handle = BGFX_INVALID_HANDLE)
      : m_handle(handle) {
    }
    BgfxUniqueHandle<T> &operator=(T handle) {
      destroy();
      m_handle = handle;
      return *this;
    }

    PX_NO_COPY(BgfxUniqueHandle)

    BgfxUniqueHandle(BgfxUniqueHandle<T> &&other) noexcept {
      m_handle = other.m_handle;
      other.m_handle = BGFX_INVALID_HANDLE;
    }
    BgfxUniqueHandle<T> &operator=(BgfxUniqueHandle<T> &&other) noexcept {
      m_handle = other.m_handle;
      other.m_handle = BGFX_INVALID_HANDLE;
      return *this;
    }

    ~BgfxUniqueHandle() {
      destroy();
    }

    operator T() const {
      return get();
    }
    T get() const {
      return m_handle;
    }

    explicit operator bool() const {
      return bgfx::isValid(m_handle);
    }

  private:
    T m_handle;

    void destroy() {
      if (bgfx::isValid(m_handle)) {
        bgfx::destroy(m_handle);
      }
    }
  };

  typedef BgfxUniqueHandle<bgfx::ProgramHandle> BgfxUniqueProgramHandle;
  typedef BgfxUniqueHandle<bgfx::ShaderHandle> BgfxUniqueShaderHandle;
  typedef BgfxUniqueHandle<bgfx::VertexBufferHandle> BgfxUniqueVertexBufferHandle;
  typedef BgfxUniqueHandle<bgfx::IndexBufferHandle> BgfxUniqueIndexBufferHandle;
  typedef BgfxUniqueHandle<bgfx::TextureHandle> BgfxUniqueTextureHandle;
  typedef BgfxUniqueHandle<bgfx::UniformHandle> BgfxUniqueUniformHandle;

} // px

#endif //ENGINE_BGFX_HANDLE_HPP
