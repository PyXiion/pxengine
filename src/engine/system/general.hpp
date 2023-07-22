#pragma once

// OS detection
#if defined(_WIN32)
  #define PX_OS_WINDOWS

#elif defined(unix) || defined(__unix) || defined(__unix__)

  #if defined(__linux__) || defined(__linux) || defined(linux)
    #define PX_OS_LINUX
  #endif

  #define PX_OS_UNIX

#elif defined(__APPLE__) || defined(__MACH__)
  #define PX_OS_MACOS

#else
  #error OS not supported

#endif

#if defined(_WIN64)
  #define PX_OS_WINDOWS64
#endif


// Is Debug or Release?
#ifdef NDEBUG
  #define PX_RELEASE
#else
  #define PX_DEBUG
#endif

// Debug macros
#ifdef PX_DEBUG
  #define PX_IF_DEBUG(...) __VA_ARGS__
  #define PX_IF_RELEASE(...)
#else
  #define PX_IF_DEBUG(...)
  #define PX_IF_RELEASE(...) __VA_ARGS__
#endif
