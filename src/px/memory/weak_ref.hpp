//
// Created by pyxiion on 15.01.24.
//

#ifndef PX_WEAK_REF_HPP
#define PX_WEAK_REF_HPP
#include "ref.hpp"
#include "ref_counting.hpp"

namespace px {
  template<class T>
  class WeakRef {
  public:
    WeakRef();

    explicit WeakRef(T *ptr);

    ~WeakRef();

    WeakRef(const Ref<T> &ref);

    WeakRef(const WeakRef &other);

    WeakRef(WeakRef &&other) noexcept;

    WeakRef &operator=(const Ref<T> &ref);

    WeakRef &operator=(const WeakRef &other);

    WeakRef &operator=(WeakRef &&other) noexcept;

    bool operator==(const WeakRef &other) const;

    bool operator==(const T *other) const;

    bool operator==(const void *other) const;

    operator bool() const;

    Ref<T> lock() const;

    void reset();

  private:
    // Pointer to the object always should go first, therefore we can access it as a pointer to another pointer
    T *m_ptr;
    RefCounting::WeakData *m_weakData;
  };

  template<class T>
  WeakRef<T>::WeakRef()
    : WeakRef(nullptr) {}

  template<class T>
  WeakRef<T>::WeakRef(T *ptr)
    : m_ptr(ptr)
  , m_weakData(nullptr) {
    if (m_ptr) {
      m_weakData = m_ptr->getWeakData();
      m_weakData->AddRef();
    }
  }

  template<class T>
  WeakRef<T>::~WeakRef() {
    if (m_weakData) {
      m_weakData->Release();
    }
  }

  template<class T>
  WeakRef<T>::WeakRef(const Ref<T> &ref)
    : WeakRef(ref.get()) {}

  template<class T>
  WeakRef<T>::WeakRef(const WeakRef &other)
    : WeakRef(other.m_ptr) {}

  template<class T>
  WeakRef<T>::WeakRef(WeakRef &&other) noexcept
    : m_ptr(other.m_ptr)
  , m_weakData(other.m_weakData) {
    other.m_ptr      = nullptr;
    other.m_weakData = nullptr;
  }

  template<class T>
  WeakRef<T> &WeakRef<T>::operator=(const Ref<T> &ref) {
    m_ptr = ref.get();
    if (m_ptr) {
      m_weakData = m_ptr->getWeakData();
      m_weakData->AddRef();
    }

    return *this;
  }

  template<class T>
  WeakRef<T> &WeakRef<T>::operator=(const WeakRef &other) {
    if (&other == this)
      return *this;

    m_ptr = other.m_ptr;
    if (m_ptr) {
      m_weakData = m_ptr->getWeakData();
      m_weakData->AddRef();
    }

    return *this;
  }

  template<class T>
  WeakRef<T> &WeakRef<T>::operator=(WeakRef &&other) noexcept {
    if (&other == this)
      return *this;

    m_ptr      = other.m_ptr;
    m_weakData = other.m_weakData;

    other.m_ptr      = nullptr;
    other.m_weakData = nullptr;

    return *this;
  }

  template<class T>
  bool WeakRef<T>::operator==(const WeakRef &other) const {
    return other.m_ptr == m_ptr;
  }

  template<class T>
  bool WeakRef<T>::operator==(const T *other) const {
    return m_ptr == other;
  }

  template<class T>
  bool WeakRef<T>::operator==(const void *other) const {
    return m_ptr == other;
  }

  template<class T>
  WeakRef<T>::operator bool() const {
    return m_weakData and not m_weakData->Get();
  }

  template<class T>
  Ref<T> WeakRef<T>::lock() const {
    if (m_weakData && not m_weakData->Get()) {
      return Ref(m_ptr);
    }
    return Ref<T>(nullptr);
  }

  template<class T>
  void WeakRef<T>::reset() {
    *this = WeakRef(nullptr);
  }
} // px

#endif //PX_WEAK_REF_HPP
