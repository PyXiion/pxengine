// Copyright (c) 2023.

//
// Created by pyxiion on 26.12.23.
//

#ifndef PX_ENGINE_AS_COUNTER_HPP
#define PX_ENGINE_AS_COUNTER_HPP

namespace px {
  namespace script {

    class AsCounter {
    public:
      AsCounter();

      void addReference();
      void releaseReference();

      [[nodiscard]] int getReferenceCount() const;

    protected:
      // constructor is protected to avoid creating objects on the stack and deleting it
      ~AsCounter() = default;

    private:

      int m_referenceCount;
    };

  } // px
} // script

#endif //PX_ENGINE_AS_COUNTER_HPP
