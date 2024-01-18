//
// Created by pyxiion on 15.01.24.
//

#ifndef PX_REF_HPP
#define PX_REF_HPP
#include <easylogging++.h>
#include <utility>

namespace px {
  class RefCounting;

  template<class T>
  class Ref final {
  public:
    Ref();

    explicit Ref(T *ptr);

    ~Ref();

    Ref(const Ref &other);

    Ref(Ref &&other) noexcept;

    auto operator=(const Ref &other) -> Ref &;

    auto operator=(Ref &&other) noexcept -> Ref &;

    auto get() const -> T *;

    auto operator*() const -> T &;

    auto operator->() const -> T *;

    explicit operator bool() const;

    template<class Base> requires std::is_base_of_v<Base, T>
    operator Ref<Base>() const;

    template<class ...TArgs>
    static auto make(TArgs && ...args) -> Ref;

  private:
    T *m_ptr{};
  };

  template<class T>
  Ref<T>::Ref()
    : Ref(nullptr) {}

  template<class T>
  Ref<T>::Ref(T *ptr)
    : m_ptr(ptr) {
    if (m_ptr) {
      m_ptr->addRef();
    }
  }

  template<class T>
  Ref<T>::~Ref() {
    if (m_ptr) {
      m_ptr->release();
    }
  }

  template<class T>
  Ref<T>::Ref(const Ref &other)
    : m_ptr(other.m_ptr) {
    if (m_ptr) {
      m_ptr->addRef();
    }
  }

  template<class T>
  Ref<T>::Ref(Ref &&other) noexcept
    : m_ptr(other.m_ptr) {
    other.m_ptr = nullptr;
  }

  template<class T>
  auto Ref<T>::operator=(const Ref &other) -> Ref & {
    if (&other == this)
      return *this;


    m_ptr = other.m_ptr;

    if (m_ptr) {
      m_ptr->addRef();
    }
    return *this;
  }

  template<class T>
  auto Ref<T>::operator=(Ref &&other) noexcept -> Ref & {
    if (&other == this)
      return *this;


    m_ptr       = other.m_ptr;
    other.m_ptr = nullptr;

    return *this;
  }

  template<class T>
  auto Ref<T>::get() const -> T * {
    return m_ptr;
  }

  template<class T>
  auto Ref<T>::operator*() const -> T & {
    return *get();
  }

  template<class T>
  auto Ref<T>::operator->() const -> T * {
    return get();
  }

  template<class T>
  Ref<T>::operator bool() const {
    return m_ptr;
  }

  template<class T>
  template<class Base> requires std::is_base_of_v<Base, T>
  Ref<T>::operator Ref<Base>() const {
    return Ref<Base>(m_ptr);
  }

  template<class T>
  template<class ...TArgs>
  auto Ref<T>::make(TArgs && ...args) -> Ref<T> {
    return Ref(new T(std::forward<TArgs>(args) ...));
  }
} // px

#endif //PX_REF_HPP
