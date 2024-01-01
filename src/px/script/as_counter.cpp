// Copyright (c) 2023.

//
// Created by pyxiion on 26.12.23.
//

#include "as_counter.hpp"

namespace px {
  namespace script {
    AsCounter::AsCounter()
      : m_referenceCount(1) {

    }

    // TODO thread safe
    void AsCounter::addReference() {
      m_referenceCount++;
    }

    void AsCounter::releaseReference() {
      m_referenceCount--;

      // object can be deleted
      if (m_referenceCount == 0) {
        delete this;
      }
    }

    int AsCounter::getReferenceCount() const {
      return m_referenceCount;
    }

  } // px
} // script