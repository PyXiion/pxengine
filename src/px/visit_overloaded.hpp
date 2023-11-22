// Copyright (c) 2023.

//
// Created by pyxiion on 05.11.23.
//

#ifndef ENGINE_VISIT_OVERLOADED_HPP
#define ENGINE_VISIT_OVERLOADED_HPP

namespace px {
  template<class... Ts>
  struct overloaded : Ts... { using Ts::operator()...; };
  template<class... Ts>
  overloaded(Ts...) -> overloaded<Ts...>;
}

#endif //ENGINE_VISIT_OVERLOADED_HPP
