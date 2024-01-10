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


  PX_NEW_EXCEPTION(RegisterError, Exception);

  PX_NEW_EXCEPTION(RegisterClassError, RegisterError);

  PX_NEW_EXCEPTION(RegisterClassTypeError, RegisterClassError);

  PX_NEW_EXCEPTION(RegisterClassBehaviourError, RegisterClassError);

  PX_NEW_EXCEPTION(RegisterClassMethodError, RegisterClassError);

  PX_NEW_EXCEPTION(RegisterClassProxyMethodError, RegisterClassMethodError);

  PX_NEW_EXCEPTION(RegisterClassCdeclMethodError, RegisterClassMethodError);

  PX_NEW_EXCEPTION(RegisterClassGenericMethodError, RegisterClassMethodError);

  PX_NEW_EXCEPTION(RegisterClassPropertyError, RegisterClassError);
}

#endif //PX_SCRIPT_EXCEPTIONS_HPP
