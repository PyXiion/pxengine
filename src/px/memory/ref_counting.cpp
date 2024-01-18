// Copyright (c) 2024.

//
// Created by pyxiion on 15.01.24.
//

#include "ref_counting.hpp"

#include <easylogging++.h>

#ifdef PX_MEMORY_USE_ANGEL_SCRIPT_ILOCKABLE_BOOL
#include <angelscript.h>
#endif

namespace px {
  #ifndef PX_MEMORY_USE_ANGEL_SCRIPT_ILOCKABLE_BOOL
  std::mutex RefCounting::createWeakRefCountMutex;

  void RefCounting::WeakData::addRef() {
    m_count += 1;
  }

  void RefCounting::WeakData::release() {
    m_count -= 1;

    if (m_count == 0) {
      delete this;
    }
  }

  std::uint32_t RefCounting::WeakData::getReferenceCount() const {
    return m_count;
  }
  #endif

  RefCounting::RefCounting()
    : m_refCount(0)
  , m_weakData(nullptr) {}

  RefCounting::~RefCounting() {
    if (m_weakData) {
      m_weakData->Set(true);
      m_weakData->Release();
    }
  }

  unsigned RefCounting::getReferenceCount() const {
    return m_refCount;
  }

  RefCounting::WeakData *RefCounting::getWeakData() const {
    if (not m_weakData) {
      #ifdef PX_MEMORY_USE_ANGEL_SCRIPT_ILOCKABLE_BOOL
      asAcquireExclusiveLock();

      if (not m_weakData) {
        m_weakData = asCreateLockableSharedBool();
      }

      asReleaseExclusiveLock();
      #else

      std::lock_guard lk(createWeakRefCountMutex);

      if (not m_weakData) {
        m_weakData = new WeakData;
      }

      #endif
    }

    return m_weakData;
  }

  bool RefCounting::hasWeakData() const {
    return m_weakData != nullptr;
  }


  void RefCounting::addRef() const {
    m_refCount += 1;
  }

  void RefCounting::release() const {
    m_refCount -= 1;

    if (m_refCount == 0) {
      m_weakData->Set(true);
      delete this;
    }
  }
} // px
