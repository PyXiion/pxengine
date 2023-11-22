// Copyright (c) 2023.

//
// Created by pyxiion on 06.11.23.
//

#ifndef ENGINE_BGFX_HANDLE_HPP
#define ENGINE_BGFX_HANDLE_HPP
#include <bgfx/bgfx.h>
#include <easy/profiler.h>

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

    BgfxUniqueHandle(const BgfxUniqueHandle<T> &other) = delete;
    BgfxUniqueHandle<T> &operator=(const BgfxUniqueHandle<T> &other) = delete;

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
      return m_handle;
    }

  private:
    T m_handle;

    inline void destroy() {
      EASY_BLOCK(__PRETTY_FUNCTION__)
      if (bgfx::isValid(m_handle))
        bgfx::destroy(m_handle);
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
