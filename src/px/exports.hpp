//
// Created by pyxiion on 22.10.23.
//

#ifndef PX_EXPORTS_HPP
#define PX_EXPORTS_HPP

#ifdef WIN32
#if BUILD_DLL
    #define PX_API declspec(dllexport)
  #else
    #define PX_API declspec(dllimport)
  #endif
#else
  #define PX_API
#endif

#endif //PX_EXPORTS_HPP
