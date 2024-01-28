// Copyright (c) 2024.

//
// Created by pyxiion on 15.01.24.
//

#include "ref_counting.hpp"

#include <easylogging++.h>


namespace px {
  std::mutex RefCounting::createWeakRefCountMutex;

  void RefCounting::WeakData::AddRef() {
    m_count += 1;
  }

  void RefCounting::WeakData::Release() {
    m_count -= 1;

    if (m_count == 0) {
      delete this;
    }
  }

  bool RefCounting::WeakData::Get() const {
    return m_isDestroyed;
  }

  void RefCounting::WeakData::Set(bool v) {
    m_isDestroyed = v;
  }

  std::uint32_t RefCounting::WeakData::getReferenceCount() const {
    return m_count;
  }

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
      std::lock_guard lk(createWeakRefCountMutex);

      if (not m_weakData) {
        m_weakData = new WeakData;
      }
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
