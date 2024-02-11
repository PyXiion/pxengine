// Copyright (c) 2024.

//
// Created by pyxiion on 11.02.24.
//

#ifndef I_REF_COUNTING_HPP
#define I_REF_COUNTING_HPP
#include "px/engine/common/frame_limiter.hpp"

namespace px {
  struct IRefCounting {
    virtual ~IRefCounting() = default;

    virtual void addRef() const = 0;

    virtual void release() const = 0;
  };
} // px

#endif //I_REF_COUNTING_HPP
