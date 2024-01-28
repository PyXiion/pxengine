//
// Created by pyxiion on 15.01.24.
//

#ifndef PX_REF_COUNTING_HPP
#define PX_REF_COUNTING_HPP
#include <atomic>


namespace px {
  class RefCounting {
  public:
    class WeakData {
    public:
      void AddRef();

      void Release();

      [[nodiscard]] bool Get() const;

      void Set(bool);

      [[nodiscard]] std::uint32_t getReferenceCount() const;

    private:
      bool m_isDestroyed{false};
      std::uint32_t m_count{1};
    };

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

    static std::mutex createWeakRefCountMutex;
  };
} // px

#endif //PX_REF_COUNTING_HPP
