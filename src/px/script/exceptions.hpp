// Copyright (c) 2024.

//
// Created by pyxiion on 02.01.24.
//

#ifndef PX_SCRIPT_EXCEPTIONS_HPP
#define PX_SCRIPT_EXCEPTIONS_HPP
#include "px/exception.hpp"

namespace px::script {
  PX_NEW_EXCEPTION(NotFound, Exception);

  PX_NEW_EXCEPTION(FunctionNotFound, NotFound);
  PX_NEW_EXCEPTION(FactoryNotFound, NotFound);
  PX_NEW_EXCEPTION(MethodNotFound, NotFound);
  PX_NEW_EXCEPTION(PropertyNotFound, NotFound);
}

#endif //PX_SCRIPT_EXCEPTIONS_HPP
