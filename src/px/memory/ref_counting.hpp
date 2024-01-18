//
// Created by pyxiion on 15.01.24.
//

#ifndef PX_REF_COUNTING_HPP
#define PX_REF_COUNTING_HPP
#include <atomic>

#ifdef PX_MEMORY_USE_ANGEL_SCRIPT_ILOCKABLE_BOOL
#include "px/script/common.hpp"
#endif


namespace px {
  class RefCounting {
  public:
    #ifndef PX_MEMORY_USE_ANGEL_SCRIPT_ILOCKABLE_BOOL
    class WeakData {
    public:
      bool isDestroyed{false};

      void AddRef();

      void Release();

      bool Get() const;

      void Set(bool) const;

      [[nodiscard]] std::uint32_t getReferenceCount() const;

    private:
      std::uint32_t m_count{1};
    };
    #else
    using WeakData = asILockableSharedBool;
    #endif

  public:
    RefCounting();

    virtual ~RefCounting();

    [[nodiscard]] unsigned int getReferenceCount() const;

    WeakData *getWeakData() const;

    bool hasWeakData() const;

    void addRef() const;

    void release() const;

  private:
    mutable std::atomic_uint32_t m_refCount;
    mutable WeakData *m_weakData;

    #ifndef PX_MEMORY_USE_ANGEL_SCRIPT_ILOCKABLE_BOOL
    static std::mutex createWeakRefCountMutex;
    #endif
  };
} // px

#endif //PX_REF_COUNTING_HPP
