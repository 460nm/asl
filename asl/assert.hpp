#pragma once

#include "asl/config.hpp"

#if ASL_COMPILER_CLANG_CL
    #define ASL_DEBUG_BREAK() __debugbreak()
#elif ASL_COMPILER_CLANG
    #define ASL_DEBUG_BREAK() __builtin_debug_trap()
#endif

#define ASL_ASSERT(...)                                                 \
    if (__VA_ARGS__) {}                                                 \
    else { ASL_DEBUG_BREAK(); }
