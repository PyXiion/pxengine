// Copyright (c) 2023.

//
// Created by pyxiion on 17.12.23.
//

#ifndef PX_ENGINE_STATIC_WARNING_HPP
#define PX_ENGINE_STATIC_WARNING_HPP

// https://stackoverflow.com/questions/8936063/does-there-exist-a-static-warning

#if defined(__GNUC__)
#define PX_DEPRECATE(foo, msg) foo __attribute__((deprecated(msg)))
#elif defined(_MSC_VER)
#define PX_DEPRECATE(foo, msg) __declspec(deprecated(msg)) foo
#else
#error This compiler is not supported
#endif

#define PX_CAT(x,y) PX_CAT1(x,y)
#define PX_CAT1(x,y) x##y

namespace detail
{
  struct true_type {};
  struct false_type {};
  template <int test> struct converter : public true_type {};
  template <> struct converter<0> : public false_type {};
}

#define PX_STATIC_WARNING(cond, msg) \
struct PX_CAT(static_warning,__LINE__) { \
  PX_DEPRECATE(void _(::detail::false_type const& ),msg) {}; \
  void _(::detail::true_type const& ) {}; \
  PX_CAT(static_warning,__LINE__)() {_(::detail::converter<(cond)>());} \
}

// Note: using STATIC_WARNING_TEMPLATE changes the meaning of a program in a small way.
// It introduces a member/variable declaration.  This means at least one byte of space
// in each structure/class instantiation.  STATIC_WARNING should be preferred in any
// non-template situation.
//  'token' must be a program-wide unique identifier.
#define PX_STATIC_WARNING_TEMPLATE(token, cond, msg) \
    STATIC_WARNING(cond, msg) PP_CAT(PP_CAT(_localvar_, token),__LINE__)


#endif //PX_ENGINE_STATIC_WARNING_HPP
