// Copyright (c) 2023.

//
// Created by pyxiion on 04.11.23.
//

#ifndef ENGINE_TEMPLATES_HPP
#define ENGINE_TEMPLATES_HPP
#include <type_traits>

namespace px {
  /// https://stackoverflow.com/questions/34672441/stdis-base-of-for-template-classes
  template <template <class...> class base, class derived>
  struct is_base_of_template_impl {
    template<class ...Ts>
    static constexpr std::true_type  test(const base<Ts...> *);
    static constexpr std::false_type test(...);

    using type = decltype(test(std::declval<derived*>()));
  };

  template <template <class...> class base, class derived>
  using is_base_of_template = typename is_base_of_template_impl<base,derived>::type;

  template <template <class...> class base, class derived>
  constexpr bool is_base_of_template_v = is_base_of_template<base,derived>::value;
}

#endif //ENGINE_TEMPLATES_HPP
