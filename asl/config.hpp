#pragma once

#if defined(_WIN32)
    #define ASL_OS_WINDOWS 1
#elif defined(__linux__)
    #define ASL_OS_LINUX 1
#else
    #error Unknown OS
#endif

#if defined(__clang__) && defined(_MSC_VER)
    #define ASL_COMPILER_CLANG_CL 1
#elif defined(__clang__)
    #define ASL_COMPILER_CLANG 1
#else
    #error Unknown compiler
#endif
