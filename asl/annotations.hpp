#pragma once

#include "asl/config.hpp"

#if ASL_COMPILER_CLANG_CL
    #define ASL_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#elif ASL_COMPILER_CLANG
    #define ASL_NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif
