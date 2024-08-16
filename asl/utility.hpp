#pragma once

#ifdef _MSVC_VER_
#elif defined(__clang_version__)
#else
    #error Not a valid environment
#endif

#ifdef _MSVC_VER_
    #define ASL_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#else
    #define ASL_NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif

